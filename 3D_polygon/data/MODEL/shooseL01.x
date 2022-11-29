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
 36;
 -1.29444;-1.94851;0.47301;,
 0.08165;-1.94851;1.61046;,
 0.08165;-1.94851;4.63872;,
 -2.67054;-1.94851;3.21905;,
 1.45774;-1.94851;0.47301;,
 2.83383;-1.94851;3.21905;,
 2.02774;-1.94851;-2.27303;,
 3.09478;-1.94851;-2.27303;,
 1.45774;-1.94851;-5.01908;,
 2.83383;-1.94851;-6.50723;,
 0.08165;-1.94851;-6.15652;,
 0.08165;-1.94851;-7.97836;,
 -1.29444;-1.94851;-5.01908;,
 -2.67054;-1.94851;-6.50723;,
 -1.86444;-1.94851;-2.27303;,
 -3.76881;-1.94851;-2.27303;,
 -2.67054;-1.94851;3.21905;,
 0.08165;-1.94851;4.63872;,
 0.08165;0.69547;2.71555;,
 -1.29444;0.69547;1.88298;,
 2.83383;-1.94851;3.21905;,
 1.45774;0.69547;1.88298;,
 3.09478;-1.94851;-2.27303;,
 2.02774;0.69547;-0.12703;,
 2.83383;-1.94851;-6.50723;,
 1.45774;0.69547;-2.13704;,
 0.08165;-1.94851;-7.97836;,
 0.08165;0.69547;-2.96961;,
 -2.67054;-1.94851;-6.50723;,
 -1.29444;0.69547;-2.13704;,
 -3.76881;-1.94851;-2.27303;,
 -1.86444;0.69547;-0.12703;,
 -2.67054;-1.94851;3.21905;,
 -1.29444;0.69547;1.88298;,
 0.08165;-1.94851;-2.27303;,
 0.08165;0.72098;-0.12703;;
 
 32;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 4;16,17,18,19;,
 4;17,20,21,18;,
 4;20,22,23,21;,
 4;22,24,25,23;,
 4;24,26,27,25;,
 4;26,28,29,27;,
 4;28,30,31,29;,
 4;30,32,33,31;,
 3;1,0,34;,
 3;4,1,34;,
 3;6,4,34;,
 3;8,6,34;,
 3;10,8,34;,
 3;12,10,34;,
 3;14,12,34;,
 3;0,14,34;,
 3;19,18,35;,
 3;18,21,35;,
 3;21,23,35;,
 3;23,25,35;,
 3;25,27,35;,
 3;27,29,35;,
 3;29,31,35;,
 3;31,33,35;;
 
 MeshMaterialList {
  1;
  32;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\murabitoUV012.png";   }
  }
 }
 MeshNormals {
  18;
  0.000000;-1.000000;-0.000000;,
  -0.365431;0.890203;0.272027;,
  0.000000;0.905013;0.425383;,
  0.385588;0.885839;0.258091;,
  0.870999;0.491280;-0.002215;,
  0.307225;0.936991;-0.166315;,
  0.000000;0.971454;-0.237227;,
  -0.263876;0.948612;-0.174659;,
  -0.443049;0.896494;0.002278;,
  -0.000000;1.000000;0.000000;,
  -0.659827;0.568151;0.491764;,
  0.000000;0.611246;0.791440;,
  0.697632;0.543512;0.466802;,
  0.587068;0.745155;-0.316377;,
  0.000000;0.887935;-0.459968;,
  -0.501980;0.798603;-0.332040;,
  -0.793933;0.607991;0.004152;,
  0.013107;0.999914;0.000000;;
  32;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;10,11,2,1;,
  4;11,12,3,2;,
  4;12,4,4,3;,
  4;4,13,5,4;,
  4;13,14,6,5;,
  4;14,15,7,6;,
  4;15,16,8,7;,
  4;16,10,1,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,2,9;,
  3;2,3,9;,
  3;3,17,9;,
  3;17,5,9;,
  3;5,6,9;,
  3;6,7,9;,
  3;7,8,9;,
  3;8,1,9;;
 }
 MeshTextureCoords {
  36;
  0.045418;0.941660;,
  0.037186;0.945070;,
  0.037186;0.956711;,
  0.053650;0.949892;,
  0.028955;0.941660;,
  0.020723;0.949892;,
  0.025545;0.933429;,
  0.013904;0.933429;,
  0.028955;0.925197;,
  0.020723;0.916965;,
  0.037186;0.921787;,
  0.037186;0.910146;,
  0.045418;0.925197;,
  0.053650;0.916965;,
  0.048828;0.933429;,
  0.060469;0.933429;,
  0.045245;0.987724;,
  0.055610;0.987724;,
  0.071156;0.946267;,
  0.065974;0.946267;,
  0.065974;0.987724;,
  0.076338;0.946267;,
  0.076338;0.987724;,
  0.081520;0.946267;,
  0.086702;0.987724;,
  0.086702;0.946267;,
  0.097067;0.987724;,
  0.091885;0.946267;,
  0.107431;0.987724;,
  0.097067;0.946267;,
  0.117795;0.987724;,
  0.102249;0.946267;,
  0.128160;0.987724;,
  0.107431;0.946267;,
  0.037186;0.933429;,
  0.086702;0.904809;;
 }
}