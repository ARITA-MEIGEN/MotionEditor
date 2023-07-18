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
 110;
 -11.27075;-2.35000;5.06300;,
 -9.97819;1.44686;5.06300;,
 -9.97819;1.44686;-5.06200;,
 -11.27075;-2.35000;-5.06200;,
 -6.44687;5.29215;5.06300;,
 -6.44687;5.29215;-5.06200;,
 -1.62300;6.30951;5.06300;,
 -1.62300;6.30951;-5.06200;,
 -1.62300;-2.35000;5.06300;,
 -1.62300;-2.35000;5.06300;,
 -1.62300;-2.35000;5.06300;,
 -1.62300;-2.35000;-5.06200;,
 -1.62300;-2.35000;-5.06200;,
 -1.62300;-2.35000;-5.06200;,
 -1.62300;-2.35000;5.06300;,
 -1.62300;-2.35000;-5.06200;,
 -1.62300;-2.35000;-5.06200;,
 -1.62300;-2.35000;5.06300;,
 -7.56787;-0.84154;1.82300;,
 -3.92287;-0.84154;1.82300;,
 -3.92287;-0.84154;-1.82200;,
 -7.56787;-0.84154;-1.82200;,
 -7.56787;-2.66404;-3.64450;,
 -3.92287;-2.66404;-3.64450;,
 -3.92287;-13.59904;-3.64450;,
 -7.56787;-13.59904;-3.64450;,
 -7.56787;-15.42155;-1.82200;,
 -3.92287;-15.42155;-1.82200;,
 -3.92287;-15.42155;1.82300;,
 -7.56787;-15.42155;1.82300;,
 -7.56787;-13.59904;3.64550;,
 -3.92287;-13.59904;3.64550;,
 -3.92287;-2.66404;3.64550;,
 -7.56787;-2.66404;3.64550;,
 -2.10037;-2.66404;1.82300;,
 -2.10037;-13.59904;1.82300;,
 -2.10037;-13.59904;-1.82200;,
 -2.10037;-2.66404;-1.82200;,
 -9.39038;-13.59904;1.82300;,
 -9.39038;-2.66404;1.82300;,
 -9.39038;-2.66404;-1.82200;,
 -9.39038;-13.59904;-1.82200;,
 -7.56787;-0.84154;-1.82200;,
 -3.92287;-0.84154;-1.82200;,
 -3.92287;-2.66404;-3.64450;,
 -7.56787;-13.59904;-3.64450;,
 -3.92287;-13.59904;-3.64450;,
 -3.92287;-15.42155;-1.82200;,
 -7.56787;-2.66404;3.64550;,
 -3.92287;-2.66404;3.64550;,
 -3.92287;-0.84154;1.82300;,
 -7.56787;-15.42155;1.82300;,
 -3.92287;-15.42155;1.82300;,
 -3.92287;-13.59904;3.64550;,
 -2.10037;-2.66404;-1.82200;,
 -2.10037;-13.59904;-1.82200;,
 -2.10037;-13.59904;1.82300;,
 -9.39038;-2.66404;-1.82200;,
 -9.39038;-2.66404;1.82300;,
 -9.39038;-13.59904;-1.82200;,
 -0.03694;-0.13481;-4.04950;,
 -0.03694;-2.15981;-3.50692;,
 -4.08694;-2.15981;-3.50692;,
 -4.08694;-0.13481;-4.04950;,
 -0.03694;-3.64220;-2.02450;,
 -4.08694;-3.64220;-2.02450;,
 -0.03694;-4.18481;0.00050;,
 -4.08694;-4.18481;0.00050;,
 -0.03694;-3.64220;2.02550;,
 -4.08694;-3.64220;2.02550;,
 -0.03694;-2.15981;3.50792;,
 -4.08694;-2.15981;3.50792;,
 -0.03694;-0.13481;4.05050;,
 -4.08694;-0.13481;4.05050;,
 -0.03694;1.89017;3.50792;,
 -4.08694;1.89017;3.50792;,
 -0.03694;3.37260;2.02550;,
 -4.08694;3.37260;2.02550;,
 -0.03694;3.91517;0.00050;,
 -4.08694;4.98094;0.00050;,
 -0.03694;3.37260;-2.02450;,
 -4.08694;3.37260;-2.02450;,
 -0.03694;1.89017;-3.50692;,
 -4.08694;1.89017;-3.50692;,
 -0.03694;-0.13481;-4.04950;,
 -4.08694;-0.13481;-4.04950;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -0.03694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;,
 -4.08694;-0.13481;0.00050;;
 
 73;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 3;8,1,0;,
 3;9,4,1;,
 3;10,6,4;,
 3;11,3,2;,
 3;12,2,5;,
 3;13,5,7;,
 4;6,14,15,7;,
 4;3,16,17,0;,
 4;18,19,20,21;,
 4;22,23,24,25;,
 4;26,27,28,29;,
 4;30,31,32,33;,
 4;34,35,36,37;,
 4;38,39,40,41;,
 4;42,43,44,22;,
 4;45,46,47,26;,
 4;48,49,50,18;,
 4;51,52,53,30;,
 4;43,50,34,54;,
 4;52,47,55,56;,
 4;18,42,57,58;,
 4;26,51,38,59;,
 4;46,44,54,55;,
 4;49,53,56,34;,
 4;22,45,59,57;,
 4;30,48,58,38;,
 3;43,54,44;,
 3;22,57,42;,
 3;46,55,47;,
 3;49,34,50;,
 3;26,59,45;,
 3;52,56,53;,
 3;18,58,48;,
 3;30,38,51;,
 4;60,61,62,63;,
 4;61,64,65,62;,
 4;64,66,67,65;,
 4;66,68,69,67;,
 4;68,70,71,69;,
 4;70,72,73,71;,
 4;72,74,75,73;,
 4;74,76,77,75;,
 4;76,78,79,77;,
 4;78,80,81,79;,
 4;80,82,83,81;,
 4;82,84,85,83;,
 3;86,61,60;,
 3;87,64,61;,
 3;88,66,64;,
 3;89,68,66;,
 3;90,70,68;,
 3;91,72,70;,
 3;92,74,72;,
 3;93,76,74;,
 3;94,78,76;,
 3;95,80,78;,
 3;96,82,80;,
 3;97,84,82;,
 3;98,63,62;,
 3;99,62,65;,
 3;100,65,67;,
 3;101,67,69;,
 3;102,69,71;,
 3;103,71,73;,
 3;104,73,75;,
 3;105,75,77;,
 3;106,77,79;,
 3;107,79,81;,
 3;108,81,83;,
 3;109,83,85;;
 
 MeshMaterialList {
  14;
  73;
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  0,
  0,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.078400;0.429600;0.800000;1.000000;;
   12.000000;
   0.120000;0.120000;0.120000;;
   0.011760;0.064440;0.120000;;
  }
  Material {
   0.213600;0.213600;0.213600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.288800;0.288800;0.288800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.166400;0.166400;0.166400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.580000;0.527200;0.392000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.511200;0.464000;0.344800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.455200;0.455200;0.455200;1.000000;;
   25.000000;
   0.220000;0.220000;0.220000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.044000;0.232000;0.432800;1.000000;;
   12.000000;
   0.120000;0.120000;0.120000;;
   0.006600;0.034800;0.064920;;
  }
  Material {
   0.024800;0.125600;0.235200;1.000000;;
   12.000000;
   0.120000;0.120000;0.120000;;
   0.003720;0.018840;0.035280;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.545600;0.128800;0.930000;;
   15.000000;
   0.020000;0.020000;0.020000;;
   0.240000;0.163680;0.038640;;
  }
  Material {
   0.718400;0.021600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  44;
  0.000000;0.000000;1.000000;,
  -0.206361;0.978476;0.000000;,
  -0.946649;0.322266;0.000000;,
  -0.860018;0.510264;0.000000;,
  -0.495052;0.868863;0.000000;,
  0.000000;0.000000;-1.000000;,
  -0.367000;0.854764;0.367001;,
  0.367001;0.854764;0.367001;,
  -0.367000;0.854764;-0.367001;,
  0.367001;0.854764;-0.367001;,
  -0.367000;0.367001;-0.854764;,
  0.367001;0.367001;-0.854763;,
  -0.367000;-0.367000;-0.854764;,
  0.367001;-0.367000;-0.854764;,
  -0.367001;-0.854763;-0.367002;,
  0.367002;-0.854762;-0.367002;,
  -0.367001;-0.854763;0.367002;,
  0.367002;-0.854762;0.367002;,
  -0.367000;-0.367000;0.854764;,
  0.367001;-0.367000;0.854764;,
  -0.367000;0.367001;0.854764;,
  0.367001;0.367001;0.854763;,
  0.854763;0.367001;0.367001;,
  0.854764;-0.367000;0.367001;,
  0.854763;0.367001;-0.367001;,
  0.854764;-0.367000;-0.367001;,
  -0.854763;-0.367000;0.367002;,
  -0.854763;0.367002;0.367002;,
  -0.854763;-0.367000;-0.367002;,
  -0.854763;0.367002;-0.367002;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000001;-1.000000;,
  0.000000;-0.500002;-0.866024;,
  0.000000;-0.866026;-0.499998;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.866026;0.499998;,
  0.000000;-0.500002;0.866024;,
  0.000000;0.000001;1.000000;,
  0.000000;0.499997;0.866027;,
  0.052531;0.809662;0.584540;,
  0.115686;0.993286;0.000000;,
  0.052531;0.809662;-0.584540;,
  0.000000;0.499997;-0.866027;,
  -1.000000;0.000000;0.000000;;
  73;
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,1,1,4;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  4;30,30,30,30;,
  4;34,34,34,34;,
  4;6,7,9,8;,
  4;10,11,13,12;,
  4;14,15,17,16;,
  4;18,19,21,20;,
  4;22,23,25,24;,
  4;26,27,29,28;,
  4;8,9,11,10;,
  4;12,13,15,14;,
  4;20,21,7,6;,
  4;16,17,19,18;,
  4;9,7,22,24;,
  4;17,15,25,23;,
  4;6,8,29,27;,
  4;14,16,26,28;,
  4;13,11,24,25;,
  4;21,19,23,22;,
  4;10,12,28,29;,
  4;18,20,27,26;,
  3;9,24,11;,
  3;10,29,8;,
  3;13,25,15;,
  3;21,22,7;,
  3;14,28,12;,
  3;17,23,19;,
  3;6,27,20;,
  3;18,26,16;,
  4;31,32,32,31;,
  4;32,33,33,32;,
  4;33,34,34,33;,
  4;34,35,35,34;,
  4;35,36,36,35;,
  4;36,37,37,36;,
  4;37,38,38,37;,
  4;38,39,39,38;,
  4;39,40,40,39;,
  4;40,41,41,40;,
  4;41,42,42,41;,
  4;42,31,31,42;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;43,43,43;,
  3;43,43,43;,
  3;43,43,43;,
  3;43,43,43;,
  3;43,43,43;,
  3;43,43,43;,
  3;43,43,43;,
  3;43,43,43;,
  3;43,43,43;,
  3;43,43,43;,
  3;43,43,43;,
  3;43,43,43;;
 }
 MeshTextureCoords {
  110;
  0.750000;0.000000;,
  0.833330;0.000000;,
  0.833330;1.000000;,
  0.750000;1.000000;,
  0.916670;0.000000;,
  0.916670;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.791670;0.000000;,
  0.875000;0.000000;,
  0.958330;0.000000;,
  0.791670;1.000000;,
  0.875000;1.000000;,
  0.958330;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.083330;0.000000;,
  0.083330;1.000000;,
  0.000000;1.000000;,
  0.166670;0.000000;,
  0.166670;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.333330;0.000000;,
  0.333330;1.000000;,
  0.416670;0.000000;,
  0.416670;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.583330;0.000000;,
  0.583330;1.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.833330;0.000000;,
  0.833330;1.000000;,
  0.916670;0.000000;,
  0.916670;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.041670;0.000000;,
  0.125000;0.000000;,
  0.208330;0.000000;,
  0.291670;0.000000;,
  0.375000;0.000000;,
  0.458330;0.000000;,
  0.541670;0.000000;,
  0.625000;0.000000;,
  0.708330;0.000000;,
  0.791670;0.000000;,
  0.875000;0.000000;,
  0.958330;0.000000;,
  0.041670;1.000000;,
  0.125000;1.000000;,
  0.208330;1.000000;,
  0.291670;1.000000;,
  0.375000;1.000000;,
  0.458330;1.000000;,
  0.541670;1.000000;,
  0.625000;1.000000;,
  0.708330;1.000000;,
  0.791670;1.000000;,
  0.875000;1.000000;,
  0.958330;1.000000;;
 }
}