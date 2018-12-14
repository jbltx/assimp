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

/** @file  FLTImporter.h
 *  @brief Declaration of the OpenFlight (*.flt) importer class.
 */
#ifndef INCLUDED_AI_OPENFLIGHT_LOADER_H
#define INCLUDED_AI_OPENFLIGHT_LOADER_H

#include <assimp/BaseImporter.h>
#include <assimp/LogAux.h>
#include <assimp/StreamReader.h>

#include <map>
struct aiNode;
struct aiMesh;
struct aiMaterial;

namespace Assimp    {

    class StreamReaderBE;

    // TinyFormatter.h
    namespace Formatter {
        template <typename T,typename TR, typename A> class basic_formatter;
        typedef class basic_formatter< char, std::char_traits<char>, std::allocator<char> > format;
    }

// -------------------------------------------------------------------------------------------
/** Importer class to load Cinema4D files using the Melange library to be obtained from
 *  www.plugincafe.com
 *
 *  Note that Melange is not free software. */
// -------------------------------------------------------------------------------------------
class FLTImporter : public BaseImporter, public LogFunctions<FLTImporter>
{
public:

    FLTImporter();
    ~FLTImporter();


public:

    // --------------------
    bool CanRead( const std::string& pFile, IOSystem* pIOHandler, bool checkSig) const;
    // --------------------
    void GetExtensionList(std::set<std::string>& extensions);

protected:

    enum opCode
    {
        ocUnsupported = 0,                                 //"An unsupported primary record - not part of the Presagis specification"
        ocHeader = 1,  //“Header Record”
        ocGroup = 2,  //“Group Record”
        ocObject = 4,  //“Object Record”
        ocFace = 5,  //“Face Record”
        ocPushLevel = 10,  //“Push Level Record”
        ocPopLevel = 11,  //“Pop Level Record”
        ocDegreeofFreedom = 14,  //“Degree of Freedom Record”
        ocPushSubface = 19,  //“Push Subface Record”
        ocPopSubface = 20,  //“Pop Subface Record”
        ocPushExtension = 21,  //“Push Extension Record”
        ocPopExtension = 22,  //“Pop Extension Record”
        ocContinuation = 23,  //“Continuation Record”
        ocComment = 31,  //“Comment Record”
        ocColorPalette = 32,  //“Color Palette Record”
        ocLongId = 33,  //“Long ID Record”
        ocMatrix = 49,  //“Matrix Record”
        ocVector = 50,  //“Vector Record”
        ocMultitexture = 52,  //“Multitexture Record”
        ocUvList = 53,  //“UV List Record”
        ocBinarySeparatingPlane = 55,  //“Binary Separating Plane Record”
        ocReplicate = 60,  //“Replicate Record”
        ocInstanceReference = 61,  //“Instance Reference Record”
        ocInstanceDefinition = 62,  //“Instance Definition Record”
        ocExternalReference = 63,  //“External Reference Record”
        ocTexturePalette = 64,  //“Texture Palette Record”
        ocVertexPalette = 67,  //“Vertex Palette Record”
        ocVertexWithColor = 68,  //“Vertex with Color Record”
        ocVertexWithColorAndNormal = 69,  //“Vertex with Color and Normal Record”
        ocVertexWithColorNormalAndUv = 70,  //“Vertex with Color, Normal and UV Record”
        ocVertexWithColorAndUv = 71,  //“Vertex with Color and UV Record”
        ocVertexList = 72,  //“Vertex List Record”
        ocLevelOfDetail = 73,  //“Level of Detail Record”
        ocBoundingBox = 74,  //“Bounding Box Record”
        ocRotateAboutEdge = 76,  //“Rotate About Edge Record”
        ocTranslate = 78,  //“Translate Record”
        ocScale = 79,  //“Scale Record”
        ocRotateAboutPoint = 80,  //“Rotate About Point Record”
        ocRotateScaleToPoint = 81,  //“Rotate and/or Scale to Point Record”
        ocPut = 82,  //“Put Record”
        ocEyepointAndTrackplanePalette = 83,  //“Eyepoint and Trackplane Palette Record”
        ocMesh = 84,  //“Mesh Record”
        ocLocalVertexPool = 85,  //“Local Vertex Pool Record”
        ocMeshPrimitive = 86,  //“Mesh Primitive Record”
        ocRoadSegment = 87,  //“Road Segment Record”
        ocRoadZone = 88,  //“Road Zone Record”
        ocMorphVertexList = 89,  //“Morph Vertex List Record”
        ocLinkagePalette = 90,  //“Linkage Palette Record”
        ocSound = 91,  //“Sound Record”
        ocRoadPath = 92,  //“Road Path Record”
        ocSoundPalette = 93,  //“Sound Palette Record”
        ocGeneralMatrix = 94,  //“General Matrix Record”
        ocText = 95,  //“Text Record”
        ocSwitch = 96,  //“Switch Record”
        ocLineStylePalette = 97,  //“Line Style Palette Record”
        ocClipRegion = 98,  //“Clip Region Record”
        ocExtension = 100,  //“Extension Record”
        ocLightSource = 101,  //“Light Source Record”
        ocLightSourcePalette = 102,  //“Light Source Palette Record”
        ocReserved103 = 103,  //“reserved"
        ocReserved104 = 104,  //“reserved"
        ocBoundingSphere = 105,  //“Bounding Sphere Record”
        ocBoundingCylinder = 106,  //“Bounding Cylinder Record”
        ocBoundingConvexHull = 107,  //“Bounding Convex Hull Record”
        ocBoundingVolumeCenter = 108,  //“Bounding Volume Center Record”
        ocBoundingVolumeOrientation = 109,  //“Bounding Volume Orientation Record”
        ocReserved110 = 110,  //“reserved"
        ocLightPoint = 111,  //“Light Point Record”
        ocTextureMappingPalette = 112,  //“Texture Mapping Palette Record”
        ocMaterialPalette = 113,  //“Material Palette Record”
        ocNameTable = 114,  //“Name Table Record”
        ocContinuouslyAdaptiveTerrain = 115,  //“CAT Record”
        ocCatData = 116,  //“CAT Data Record”
        ocReserved117 = 117,  //“reserved"
        ocReserved118 = 118,  //“reserved"
        ocBoundingHistogram = 119,  //“Bounding Histogram Record”
        ocReserved120 = 120,  //“reserved"
        ocReserved121 = 121,  //“reserved"
        ocPushAttribute = 122,  //“Push Attribute Record”
        ocPopAttribute = 123,  //“Pop Attribute Record”
        ocReserved124 = 124,  //“reserved"
        ocReserved125 = 125,  //“reserved"
        ocCurve = 126,  //“Curve Record”
        ocRoadConstruction = 127,  //“Road Construction Record”
        ocLightPointAppearancePalette = 128,  //“Light Point Appearance Palette Record”
        ocLightPointAnimationPalette = 129,  //“Light Point Animation Palette Record”
        ocIndexedLightPoint = 130,  //“Indexed Light Point Record”
        ocLightPointSystem = 131,  //“Light Point System Record”
        ocIndexedString = 132,  //“Indexed String Record”
        ocShaderPalette = 133,  //“Shader Palette Record”
        ocReserved134 = 134,  //“reserved"
        ocExtendedMaterialHeader = 135,  //“Extended Material Header Record”
        ocExtendedMaterialAmbient = 136,  //“Extended Material Ambient Record”
        ocExtendedMaterialDiffuse = 137,  //“Extended Material Diffuse Record”
        ocExtendedMaterialSpecular = 138,  //“Extended Material Specular Record”
        ocExtendedMaterialEmissive = 139,  //“Extended Material Emissive Record”
        ocExtendedMaterialAlpha = 140,  //“Extended Material Alpha Record”
        ocExtendedMaterialLightMap = 141,  //“Extended Material Light Map Record”
        ocExtendedMaterialNormalMap = 142,  //“Extended Material Normal Map Record”
        ocExtendedMaterialBumpMap = 143,  //“Extended Material Bump Map Record”
        ocReserved144 = 144,  //“reserved"
        ocExtendedMaterialShadowMap = 145,  //“Extended Material Shadow Map Record”
        ocReserved146 = 146,  //“reserved"
        ocExtendedMaterialReflectionMap = 147,  //“Extended Material Reflection Map Record”
        ocExtensionGuidPalette = 148,  //“Extension GUID Palette Record”
        ocExtensionFieldBoolean = 149,  //“Extension Field Boolean Record”
        ocExtensionFieldInteger = 150,  //“Extension Field Integer Record”
        ocExtensionFieldFloat = 151,  //“Extension Field Float Record”
        ocExtensionFieldDouble = 152,  //“Extension Field Double Record”
        ocExtensionFieldString = 153,  //“Extension Field String Record”
        ocExtensionFieldXmlString = 154   //“Extension Field XML String Record”
    };


    enum obSoleteOpCode
    {
        oocLevelofDetail = 3,     //(single precision floating point, replaced by Opcode 73)
        oocVertexWithId = 6,     //(scaled integer coordinates, replaced by Opcodes 68-71)
        oocShortVertexWhitoutId = 7,     //(scaled integer coordinates, replaced by Opcodes 68-71)
        oocVertexWithColor = 8,     //(scaled integer coordinates, replaced by Opcodes 68-71)
        oocVertexWithColorAndNormal = 9,     //(scaled integer coordinates, replaced by Opcodes 68-71)
        oocTranslate12 = 12,     //(replaced by Opcode 78)
        oocDegreeofFreedom = 13,     //(scaled integer coordinates, replaced by Opcode 14)
        oocInstanceReference = 16,     //(replaced by Opcode 61)
        oocInstanceDefinition = 17,     //(replaced by Opcode 62)
        oocTranslate40 = 40,     //(replaced by Opcode 78)
        oocRotateAboutPoint41 = 41,     //(replaced by Opcode 80)
        oocRotateAboutEdge = 42,     //(replaced by Opcode 76)
        oocScale43 = 43,     //(replaced by Opcode 79)
        oocTranslate44 = 44,     //(replaced by Opcode 78)
        oocScaleNonuniform = 45,     //(replaced by Opcode 79)
        oocRotateAboutPoint46 = 46,     //(replaced by Opcode 80)
        oocRotateAndScaleToPoint = 47,     //(replaced by Opcode 81)
        oocPut = 48,     //(replaced by Opcode 82)
        oocBoundingBox = 51,     //(replaced by Opcode 74)
        oocEyepointPalette = 65,     //(only eyepoints, replaced by Opcode 83)
        oocMaterialPalette = 66,     //(fixed size 64 entries, replaced by Opcode 80)
        oocScale77 = 77      //(replaced by Opcode 79)
    };

    // --------------------
    void InternReadFile( const std::string& pFile, aiScene* pScene, IOSystem* pIOHandler);
    // --------------------
    void readRecord(StreamReaderBE &reader);
    

private:
    std::vector<aiMesh*> m_meshes;
    std::vector<aiMaterial*> m_materials;

}; // !class FLTImporter

} // end of namespace Assimp
#endif // INCLUDED_AI_OPENFLIGHT_LOADER_H

