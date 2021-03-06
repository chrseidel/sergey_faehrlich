xof 0302txt 0032
Header {
 1;
 0;
 1;
}
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template FrameTransformMatrix {
 <F6F23F41-7686-11cf-8F52-0040333594A3>
 Matrix4x4 frameMatrix;
}

template Frame {
 <3D82AB46-62DA-11cf-AB39-0020AF71E433>
 [...]
}
template FloatKeys {
 <10DD46A9-775B-11cf-8F52-0040333594A3>
 DWORD nValues;
 array FLOAT values[nValues];
}

template TimedFloatKeys {
 <F406B180-7B3B-11cf-8F52-0040333594A3>
 DWORD time;
 FloatKeys tfkeys;
}

template AnimationKey {
 <10DD46A8-775B-11cf-8F52-0040333594A3>
 DWORD keyType;
 DWORD nKeys;
 array TimedFloatKeys keys[nKeys];
}

template AnimationOptions {
 <E2BF56C0-840F-11cf-8F52-0040333594A3>
 DWORD openclosed;
 DWORD positionquality;
}

template Animation {
 <3D82AB4F-62DA-11cf-AB39-0020AF71E433>
 [...]
}

template AnimationSet {
 <3D82AB50-62DA-11cf-AB39-0020AF71E433>
 [Animation]
}

template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template VertexDuplicationIndices {
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}


Mesh M_Sphere_0 {
 64;
0.000000; -0.400000; 0.000000;,
0.000000; 0.400000; 0.000000;,
0.235114; -0.323607; 0.000000;,
0.190211; -0.323607; 0.138197;,
0.072654; -0.323607; 0.223607;,
-0.072654; -0.323607; 0.223607;,
-0.190211; -0.323607; 0.138197;,
-0.235114; -0.323607; -0.000000;,
-0.190211; -0.323607; -0.138197;,
-0.072654; -0.323607; -0.223607;,
0.072654; -0.323607; -0.223607;,
0.190211; -0.323607; -0.138197;,
0.380423; -0.123607; 0.000000;,
0.307768; -0.123607; 0.223607;,
0.117557; -0.123607; 0.361803;,
-0.117557; -0.123607; 0.361803;,
-0.307768; -0.123607; 0.223607;,
-0.380423; -0.123607; -0.000000;,
-0.307768; -0.123607; -0.223607;,
-0.117557; -0.123607; -0.361803;,
0.117557; -0.123607; -0.361803;,
0.307768; -0.123607; -0.223607;,
0.380423; 0.123607; 0.000000;,
0.307768; 0.123607; 0.223607;,
0.117557; 0.123607; 0.361803;,
-0.117557; 0.123607; 0.361803;,
-0.307768; 0.123607; 0.223607;,
-0.380423; 0.123607; -0.000000;,
-0.307768; 0.123607; -0.223607;,
-0.117557; 0.123607; -0.361803;,
0.117557; 0.123607; -0.361803;,
0.307768; 0.123607; -0.223607;,
0.235114; 0.323607; 0.000000;,
0.190211; 0.323607; 0.138197;,
0.072654; 0.323607; 0.223607;,
-0.072654; 0.323607; 0.223607;,
-0.190211; 0.323607; 0.138197;,
-0.235114; 0.323607; -0.000000;,
-0.190211; 0.323607; -0.138197;,
-0.072654; 0.323607; -0.223607;,
0.072654; 0.323607; -0.223607;,
0.190211; 0.323607; -0.138197;,
0.000000; -0.400000; 0.000000;,
0.000000; -0.400000; 0.000000;,
0.000000; -0.400000; 0.000000;,
0.000000; -0.400000; 0.000000;,
0.000000; -0.400000; 0.000000;,
0.000000; -0.400000; 0.000000;,
0.000000; -0.400000; 0.000000;,
0.000000; -0.400000; 0.000000;,
0.000000; -0.400000; 0.000000;,
0.000000; 0.400000; 0.000000;,
0.000000; 0.400000; 0.000000;,
0.000000; 0.400000; 0.000000;,
0.000000; 0.400000; 0.000000;,
0.000000; 0.400000; 0.000000;,
0.000000; 0.400000; 0.000000;,
0.000000; 0.400000; 0.000000;,
0.000000; 0.400000; 0.000000;,
0.000000; 0.400000; 0.000000;,
0.235114; -0.323607; 0.000000;,
0.380423; -0.123607; 0.000000;,
0.380423; 0.123607; 0.000000;,
0.235114; 0.323607; 0.000000;;

 80;
3;2,12,13;,
3;3,13,14;,
3;4,14,15;,
3;5,15,16;,
3;6,16,17;,
3;7,17,18;,
3;8,18,19;,
3;9,19,20;,
3;10,20,21;,
3;11,21,61;,
3;12,22,23;,
3;13,23,24;,
3;14,24,25;,
3;15,25,26;,
3;16,26,27;,
3;17,27,28;,
3;18,28,29;,
3;19,29,30;,
3;20,30,31;,
3;21,31,62;,
3;22,32,33;,
3;23,33,34;,
3;24,34,35;,
3;25,35,36;,
3;26,36,37;,
3;27,37,38;,
3;28,38,39;,
3;29,39,40;,
3;30,40,41;,
3;31,41,63;,
3;0,2,3;,
3;33,32,1;,
3;0,3,4;,
3;34,33,1;,
3;0,4,5;,
3;35,34,1;,
3;0,5,6;,
3;36,35,1;,
3;0,6,7;,
3;37,36,1;,
3;46,7,8;,
3;38,37,55;,
3;47,8,9;,
3;39,38,56;,
3;48,9,10;,
3;40,39,57;,
3;49,10,11;,
3;41,40,58;,
3;50,11,60;,
3;63,41,59;,
3;2,13,3;,
3;3,14,4;,
3;4,15,5;,
3;5,16,6;,
3;6,17,7;,
3;7,18,8;,
3;8,19,9;,
3;9,20,10;,
3;10,21,11;,
3;11,61,60;,
3;12,23,13;,
3;13,24,14;,
3;14,25,15;,
3;15,26,16;,
3;16,27,17;,
3;17,28,18;,
3;18,29,19;,
3;19,30,20;,
3;20,31,21;,
3;21,62,61;,
3;22,33,23;,
3;23,34,24;,
3;24,35,25;,
3;25,36,26;,
3;26,37,27;,
3;27,38,28;,
3;28,39,29;,
3;29,40,30;,
3;30,41,31;,
3;31,63,62;;

MeshMaterialList {
 1;
 80;
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0,
 0;;
Material {
0.000000; 255.000000; 0.000000; 255.000000;;
0.000000;
0.000000; 0.000000; 0.000000;;
0.000000; 0.000000; 0.000000;;
}
}
MeshTextureCoords {
 64;
0.000000; 1.000000;,
0.000000; 0.000000;,
0.000000; 0.800000;,
0.100000; 0.800000;,
0.200000; 0.800000;,
0.300000; 0.800000;,
0.400000; 0.800000;,
0.500000; 0.800000;,
0.600000; 0.800000;,
0.700000; 0.800000;,
0.800000; 0.800000;,
0.900000; 0.800000;,
0.000000; 0.600000;,
0.100000; 0.600000;,
0.200000; 0.600000;,
0.300000; 0.600000;,
0.400000; 0.600000;,
0.500000; 0.600000;,
0.600000; 0.600000;,
0.700000; 0.600000;,
0.800000; 0.600000;,
0.900000; 0.600000;,
0.000000; 0.400000;,
0.100000; 0.400000;,
0.200000; 0.400000;,
0.300000; 0.400000;,
0.400000; 0.400000;,
0.500000; 0.400000;,
0.600000; 0.400000;,
0.700000; 0.400000;,
0.800000; 0.400000;,
0.900000; 0.400000;,
0.000000; 0.200000;,
0.100000; 0.200000;,
0.200000; 0.200000;,
0.300000; 0.200000;,
0.400000; 0.200000;,
0.500000; 0.200000;,
0.600000; 0.200000;,
0.700000; 0.200000;,
0.800000; 0.200000;,
0.900000; 0.200000;,
0.100000; 1.000000;,
0.200000; 1.000000;,
0.300000; 1.000000;,
0.400000; 1.000000;,
0.500000; 1.000000;,
0.600000; 1.000000;,
0.700000; 1.000000;,
0.800000; 1.000000;,
0.900000; 1.000000;,
0.100000; 0.000000;,
0.200000; 0.000000;,
0.300000; 0.000000;,
0.400000; 0.000000;,
0.500000; 0.000000;,
0.600000; 0.000000;,
0.700000; 0.000000;,
0.800000; 0.000000;,
0.900000; 0.000000;,
1.000000; 0.800000;,
1.000000; 0.600000;,
1.000000; 0.400000;,
1.000000; 0.200000;;
}
}
