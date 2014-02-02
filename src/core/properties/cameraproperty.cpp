/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Link�ping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/interaction/events/resizeevent.h>
#include <inviwo/core/util/glmstreamoperators.h>

namespace inviwo {

CameraProperty::CameraProperty(std::string identifier, std::string displayName,
                               vec3 eye, vec3 center, vec3 lookUp,
                               Inport* inport,
                               PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics semantics)
    : CompositeProperty(identifier, displayName, invalidationLevel, semantics), EventListener()
    , lookFrom_("lookFrom", "Look from", eye, -vec3(10.0f), vec3(10.0f), vec3(0.1f), invalidationLevel)
    , lookTo_("lookTo", "Look to", center, -vec3(10.0f), vec3(10.0f), vec3(0.1f), invalidationLevel)
    , lookUp_("lookUp", "Look up", lookUp, -vec3(10.0f), vec3(10.0f), vec3(0.1f), invalidationLevel)
    , fovy_("fov", "FOV", 60.0f, 30.0f, 360.0f, 0.1f, invalidationLevel)
    , aspectRatio_("aspectRatio", "Aspect Ratio", 256.0f/256.0f, 0.0f, 1.0f, 0.1f, invalidationLevel)
    , farPlane_("far", "Far Plane", 100.0f, 1.0f, 1000.0f, 1.0f, invalidationLevel)
    , nearPlane_("near", "Near Plane", 0.1f, 0.001f, 10.f, 0.001f, invalidationLevel)
    , lockInvalidation_(false)
    , inport_(inport)
    , data_(0) {
    lookFrom_.onChange(this, &CameraProperty::updateViewMatrix);
    lookTo_.onChange(this, &CameraProperty::updateViewMatrix);
    lookUp_.onChange(this, &CameraProperty::updateViewMatrix);
    addProperty(lookFrom_);
    addProperty(lookTo_);
    addProperty(lookUp_);
    fovy_.onChange(this, &CameraProperty::updateProjectionMatrix);
    aspectRatio_.onChange(this, &CameraProperty::updateProjectionMatrix);
    nearPlane_.onChange(this, &CameraProperty::updateProjectionMatrix);
    farPlane_.onChange(this, &CameraProperty::updateProjectionMatrix);
    addProperty(fovy_);
    addProperty(aspectRatio_);
    addProperty(nearPlane_);
    addProperty(farPlane_);
    lockInvalidation();
    updateViewMatrix();
    updateProjectionMatrix();
    unlockInvalidation();

    if (inport_)
        inport_->onChange(this,&CameraProperty::inportChanged);
}

CameraProperty::~CameraProperty() {}

void CameraProperty::setLookFrom(vec3 lookFrom) {
    lookFrom_.set(lookFrom);
}

void CameraProperty::setLookTo(vec3 lookTo) {
    lookTo_.set(lookTo);
}

void CameraProperty::setLookUp(vec3 lookUp) {
    lookUp_.set(lookUp);
}

void CameraProperty::setLook(vec3 lookFrom, vec3 lookTo, vec3 lookUp) {
    bool lock = isInvalidationLocked();

    if (!lock)
        lockInvalidation();

    lookFrom_.set(lookFrom);
    lookTo_.set(lookTo);
    lookUp_.set(lookUp);

    if (!lock)
        unlockInvalidation();

    updateViewMatrix();
}

float CameraProperty::getNearPlaneDist() const {
    return nearPlane_.get();
}

float CameraProperty::getFarPlaneDist() const {
    return farPlane_.get();
}

//XY between -1 -> 1, Z between 0 -> 1
vec3 CameraProperty::getWorldPosFromNormalizedDeviceCoords(vec3 ndcCoords) const {
    float clipW = projectionMatrix_[2][3] / (ndcCoords.z - (projectionMatrix_[2][2] / projectionMatrix_[3][2]));
    vec4 clipCoords = vec4(ndcCoords * clipW, clipW);
    vec4 eyeCoords = inverseProjectionMatrix()*clipCoords;
    vec4 worldCoords = inverseViewMatrix()*eyeCoords;
    worldCoords /= worldCoords.w;
    return worldCoords.xyz();
}

void CameraProperty::setProjectionMatrix(float fovy, float aspect, float nearPlane, float farPlane) {
    fovy_.set(fovy);
    aspectRatio_.set(aspect);
    farPlane_.set(farPlane);
    nearPlane_.set(nearPlane);
    updateProjectionMatrix();
}

void CameraProperty::updateProjectionMatrix() {
    projectionMatrix_ = glm::perspective(fovy_.get(), aspectRatio_.get(), getNearPlaneDist(), getFarPlaneDist());
    inverseProjectionMatrix_ = glm::inverse(projectionMatrix_);
    invalidate();
}

void CameraProperty::updateViewMatrix() {
    lookRight_ = glm::cross(lookUp_.get(), glm::normalize(lookTo_.get()-lookFrom_.get()));
    viewMatrix_ = glm::lookAt(lookFrom_.get(), lookTo_.get(), glm::normalize(lookUp_.get()));
    inverseViewMatrix_ = glm::inverse(viewMatrix_);
    invalidate();
}

void CameraProperty::invalidate() {
    if (!isInvalidationLocked())
        Property::propertyModified();
}

void CameraProperty::invokeEvent(Event* event) {
    ResizeEvent* resizeEvent = dynamic_cast<ResizeEvent*>(event);

    if (resizeEvent) {
        uvec2 canvasSize = resizeEvent->size();
        float width = (float)canvasSize[0];
        float height = (float)canvasSize[1];
        setProjectionMatrix(fovy_.get(), width/height, nearPlane_.get(), farPlane_.get());
    }
}

void CameraProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("lookFrom", lookFrom_);
    s.serialize("lookTo", lookTo_);
    s.serialize("lookUp", lookUp_);
    s.serialize("fovy", fovy_);
    s.serialize("aspectRatio", aspectRatio_);
    s.serialize("nearPlane", nearPlane_);
    s.serialize("farPlane", farPlane_);
}

void CameraProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    d.deserialize("lookFrom", lookFrom_);
    d.deserialize("lookTo", lookTo_);
    d.deserialize("lookUp", lookUp_);
    d.deserialize("fovy", fovy_);
    d.deserialize("aspectRatio", aspectRatio_);
    d.deserialize("nearPlane", nearPlane_);
    d.deserialize("farPlane", farPlane_);
    lockInvalidation();
    updateViewMatrix();
    updateProjectionMatrix();
    unlockInvalidation();
}

void CameraProperty::setInport(Inport* inport) {
    if (inport_ != inport)
        inport->onChange(this,&CameraProperty::inportChanged);

    inport_ = inport;
}

void CameraProperty::fitCameraToVolume(const Volume* volume) {
    mat3 newBasis = volume->getBasis();
    fitWithBasis(newBasis);
}

void CameraProperty::fitCameraToGeomtry(const Geometry* geometry) {
    mat3 newBasis = geometry->getBasis();
    fitWithBasis(newBasis);
}

void CameraProperty::fitWithBasis(const mat3& basis) {
    lockInvalidation();
    float newSize = glm::length(basis * vec3(2,2,2));
    float oldSize = glm::length(oldBasis_ * vec3(2,2,2));
    float ratio = newSize/oldSize;

    if (ratio == 1)
        return;

    float newFarPlane = farPlane_.get()*ratio;
    farPlane_.setMaxValue(farPlane_.getMaxValue() * ratio);
    farPlane_.set(newFarPlane);
    vec3 oldOffset = lookFrom_.get() - lookTo_.get();
    vec3 newPos    = lookTo_.get() + (oldOffset * ratio);
    float l1 = glm::length(oldOffset);
    float l2 = glm::length(newPos - lookTo_.get());
    lookFrom_.setMinValue(lookFrom_.getMinValue()*ratio);
    lookFrom_.setMaxValue(lookFrom_.getMaxValue()*ratio);
    lookTo_.setMinValue(lookTo_.getMinValue()*ratio);
    lookTo_.setMaxValue(lookTo_.getMaxValue()*ratio);
    lookFrom_.set(newPos); //TODO something wrong is happening in here.
    updateViewMatrix();
    updateProjectionMatrix();
    unlockInvalidation();
    oldBasis_ = basis;
}

void CameraProperty::inportChanged() {
    VolumeInport* volumeInport = dynamic_cast<VolumeInport*>(inport_);
    GeometryInport* geometryInport = dynamic_cast<GeometryInport*>(inport_);
    const SpatialEntity<3>* data = 0; //using SpatialEntity since Geometry is not derived from data

    if (volumeInport) {
        const Volume* newVolume = volumeInport->getData();
        data = newVolume;
    } else if (geometryInport) {
        const Geometry* newGeometry = geometryInport->getData();
        data = newGeometry;
    }

    if (data_ == 0) { // first time only
        data_ = data;

        if (volumeInport)
            oldBasis_ = volumeInport->getData()->getBasis();
        else if (geometryInport)
            oldBasis_ = geometryInport->getData()->getBasis();

        return;
    }

    if (data_ == data) // nothing changed change
        return;

    if (volumeInport)
        fitCameraToVolume(volumeInport->getData());
    else if (geometryInport)
        fitCameraToGeomtry(geometryInport->getData());

    data_ = data;
}


} // namespace
