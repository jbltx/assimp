/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

/** @file  FLTImporter.cpp
 *  @brief Implementation of the OpenFlight importer class.
 */
#ifndef ASSIMP_BUILD_NO_FLT_IMPORTER

#ifndef _MSC_VER
#   error FLT support is currently MSVC only
#endif

#include "FLTImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/TinyFormatter.h>
#include <memory>
#include <assimp/IOSystem.hpp>
#include <assimp/scene.h>
#include <assimp/ai_assert.h>
#include <assimp/SceneCombiner.h>

#if defined(_M_X64) || defined(__amd64__)
#   define __FLT_64BIT
#endif

#define __PC

using namespace Assimp;
using namespace Assimp::Formatter;



// ------------------------------------------------------------------------------------------------
FLTImporter::FLTImporter()
{}

// ------------------------------------------------------------------------------------------------
FLTImporter::~FLTImporter()
{}

// ------------------------------------------------------------------------------------------------
bool FLTImporter::CanRead( const std::string& pFile, IOSystem* pIOHandler, bool checkSig) const
{
    const std::string& extension = GetExtension(pFile);
    if (extension == "flt") {
        return true;
    }
    return false;
}

void Assimp::FLTImporter::GetExtensionList(std::set<std::string>& extensions)
{
    extensions.insert("flt");
}

// ------------------------------------------------------------------------------------------------
// Imports the given file into the given scene structure.
void FLTImporter::InternReadFile( const std::string& pFile,
    aiScene* pScene, IOSystem* pIOHandler)
{
    IOStream *file(pIOHandler->Open(pFile, "rb"));

    if (file == nullptr)
    {
        throw DeadlyImportError("Failed to read the file " + pFile + ".");
    }


    StreamReaderBE reader(file);

    while (reader.GetRemainingSize() > 0)
    {
        readRecord(reader);
    }

    delete file;
}

// ------------------------------------------------------------------------------------------------
void Assimp::FLTImporter::readRecord(StreamReaderBE &reader)
{
    size_t startPos = reader.GetCurrentPos();

    uint16_t opCode = reader.GetU2();
    uint16_t recordLength = reader.GetU2();


    reader.SetCurrentPos(startPos + recordLength);
}


#endif // ASSIMP_BUILD_NO_FLT_IMPORTER

