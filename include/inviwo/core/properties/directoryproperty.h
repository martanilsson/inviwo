/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2019 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#ifndef IVW_DIRECTORY_PROPERTY_H
#define IVW_DIRECTORY_PROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/fileproperty.h>

namespace inviwo {

/**
 * \ingroup properties
 * A class for file representations.
 * Holds the value of the path to a file as a string.
 * @see FileProperty, StringProperty
 */
class IVW_CORE_API DirectoryProperty : public FileProperty {
public:
    virtual std::string getClassIdentifier() const override;
    static const std::string classIdentifier;

    DirectoryProperty(std::string identifier, std::string displayName, std::string value = "",
                      std::string contentType = "default",
                      InvalidationLevel invalidationLevel = InvalidationLevel::InvalidOutput,
                      PropertySemantics semantics = PropertySemantics::Default);

    DirectoryProperty(const DirectoryProperty& rhs);
    DirectoryProperty& operator=(const DirectoryProperty& that);
    virtual DirectoryProperty* clone() const override;

    // Override the FileProperty's function requestFile() to load a directory instead of a single
    // file.
    virtual void requestFile() override;

    virtual ~DirectoryProperty();

    virtual std::string getClassIdentifierForWidget() const override;
};

}  // namespace inviwo

#endif  // IVW_DIRECTORY_PROPERTY_H
