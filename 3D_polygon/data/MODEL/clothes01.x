xof 0302txt 0064
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

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
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

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
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

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 31;
 -11.29865;-11.84521;8.36712;,
 -0.48531;-11.84521;11.94622;,
 -0.48531;11.96797;6.53553;,
 -6.17220;11.35782;5.87142;,
 10.32804;-11.84521;8.36712;,
 5.20158;11.27454;5.87142;,
 14.80707;-11.84521;-0.27359;,
 7.44743;11.96851;-0.27358;,
 14.80707;-11.84521;-0.27359;,
 10.32804;-11.84521;-8.91429;,
 5.20158;11.55742;-6.41857;,
 7.44743;11.96851;-0.27358;,
 -0.48531;-11.84521;-12.49340;,
 -0.48531;11.78697;-8.96391;,
 -11.29865;-11.84521;-8.91430;,
 -6.17220;11.74391;-6.41857;,
 -15.77769;-11.84521;-0.27359;,
 -8.41806;12.15500;-0.27358;,
 -15.77769;-11.84521;-0.27359;,
 -8.41806;12.15500;-0.27358;,
 -0.48531;-11.84521;11.94622;,
 -11.29865;-11.84521;8.36712;,
 -0.48531;-4.77622;-0.27358;,
 10.32804;-11.84521;8.36712;,
 14.80707;-11.84521;-0.27359;,
 10.32804;-11.84521;-8.91429;,
 -0.48531;-11.84521;-12.49340;,
 -11.29865;-11.84521;-8.91430;,
 -15.77769;-11.84521;-0.27359;,
 -0.48531;11.19890;-0.27359;,
 -0.48531;11.19890;-0.27359;;
 
 24;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;8,9,10,11;,
 4;9,12,13,10;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;18,0,3,19;,
 3;20,21,22;,
 3;23,20,22;,
 3;24,23,22;,
 3;25,24,22;,
 3;26,25,22;,
 3;27,26,22;,
 3;28,27,22;,
 3;21,28,22;,
 3;3,2,29;,
 3;2,5,29;,
 3;5,7,29;,
 3;11,10,30;,
 3;10,13,30;,
 3;13,15,30;,
 3;15,17,30;,
 3;19,3,29;;
 
 MeshMaterialList {
  1;
  24;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\murabitoUV012.png";   }
  }
 }
 MeshNormals {
  26;
  -0.609702;0.249765;0.752251;,
  0.000787;0.200335;0.979727;,
  0.610843;0.250813;0.750975;,
  0.960761;0.277355;0.003438;,
  0.670221;0.216734;-0.709810;,
  0.000994;0.161987;-0.986792;,
  -0.669746;0.215540;-0.710622;,
  -0.961263;0.275598;0.004456;,
  -0.000000;-1.000000;-0.000000;,
  0.011723;0.999928;0.002438;,
  0.000000;-0.865599;-0.500737;,
  0.291953;-0.886230;-0.359666;,
  -0.291953;-0.886230;-0.359666;,
  -0.419595;-0.907712;0.000000;,
  -0.291953;-0.886230;0.359665;,
  -0.000000;-0.865600;0.500737;,
  0.291953;-0.886230;0.359665;,
  0.419595;-0.907712;-0.000000;,
  0.013203;0.999820;-0.013638;,
  0.007202;0.993656;-0.112231;,
  0.005632;0.999831;-0.017519;,
  -0.096538;0.995067;0.022836;,
  -0.043346;0.998895;0.018165;,
  0.016356;0.997585;0.067506;,
  0.071300;0.997202;0.022459;,
  0.119602;0.992335;0.031084;;
  24;
  4;0,1,1,0;,
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,0,0,7;,
  3;10,11,8;,
  3;12,10,8;,
  3;13,12,8;,
  3;14,13,8;,
  3;15,14,8;,
  3;16,15,8;,
  3;17,16,8;,
  3;11,17,8;,
  3;18,19,9;,
  3;19,20,9;,
  3;20,21,9;,
  3;21,22,9;,
  3;22,23,9;,
  3;23,24,9;,
  3;24,25,9;,
  3;25,18,9;;
 }
 MeshTextureCoords {
  31;
  0.896627;0.844363;,
  0.782656;0.844363;,
  0.782656;0.524517;,
  0.842595;0.532712;,
  0.668685;0.844363;,
  0.722717;0.533830;,
  0.621477;0.844363;,
  0.699047;0.524509;,
  0.542367;0.476948;,
  0.607740;0.476948;,
  0.650659;0.121780;,
  0.561628;0.115541;,
  0.748037;0.476948;,
  0.748037;0.118296;,
  0.888333;0.476948;,
  0.821820;0.118949;,
  0.972690;0.476948;,
  0.909878;0.112711;,
  0.983410;0.844363;,
  0.898074;0.522005;,
  0.422744;0.743176;,
  0.415006;0.716585;,
  0.386777;0.739220;,
  0.409412;0.767450;,
  0.382821;0.775187;,
  0.358547;0.761856;,
  0.350810;0.735265;,
  0.364141;0.710991;,
  0.393399;0.679001;,
  0.782656;0.534846;,
  0.748037;0.127221;;
 }
}