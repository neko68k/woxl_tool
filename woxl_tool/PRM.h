#ifndef __PRM_H__
#define __PRM_H__
#include "stdafx.h"
#include "binfile.h"
#include "endian_tools.h"
#include <vector>

typedef struct {
    unsigned char name[15]; /* Object name */
    unsigned char unknown1;
    unsigned short vtx_count; /* Number of vertices */
    unsigned short unknown2[7];
    unsigned short poly_count; /* Number of polygones */
    unsigned short unknown3[10];
    unsigned short index1;
    unsigned short unknown4[14];
    long position_x; /* Position of object relative to origin */
    long position_y;
    long position_z;
    unsigned short unknown5[10];
    long objcoord2[3];
    unsigned short unknown6[8];
} prm_object_header_t;

//Vertices
//After the object header comes objects vertices, as an array of prm_object_vertex_t[vtx_count].
typedef struct {
    short x;
    short y;
    short z;
    short unknown;  
} prm_object_vertex_t;

//Polygons
//After the array of vertices comes polygons. Polygons have variable length, depending on their type, which is the first value read:
typedef struct {
    unsigned short type; /* Polygon type */
} prm_object_polygon_type_t;


//Then the polygon data follows the type:
//Polygon type 0x01
typedef struct {
	unsigned short unknown1;
	unsigned short vertices[3];	/* Vertices index */
	unsigned short unknown2[3];
} prm_object_polygon_1_t;

//Polygon type 0x02
typedef struct {
	unsigned short unknown1;
	unsigned short vertices[3];	/* Vertices index */
	unsigned short colour_index;
	unsigned short unknown2[3];
} prm_object_polygon_2_t;

//Polygon type 0x03
typedef struct {
	unsigned short unknown1;
	unsigned short vertices[4];	/* Vertices index */
	unsigned short unknown2[2];
} prm_object_polygon_3_t;

//Polygon type 0x04 : Quad
typedef struct {
	unsigned short unknown1;
	unsigned short vertices[4];	/* Vertices index */
	unsigned short colour_index;
	unsigned short unknown2[2];
} prm_object_polygon_4_t;

//Polygon type 0x05 : Gouraud triangle
typedef struct {
	unsigned short unknown1;
	unsigned short vertices[3];	/* Vertices index */
	unsigned short unknown2;
	unsigned long colours[3];
} prm_object_polygon_5_t;

//Polygon type 0x06
//There are several types of polygon type 6. As for polygon type, they start with it.
typedef struct {
	unsigned short type;
} prm_object_polygon_type_6_t;

//Then follow various subtypes:
//Polygon type 0x06, subtype 0x00,0x01
typedef struct {
	unsigned short vertices[3];	/* Vertices index */
	unsigned short texture_index;
	unsigned short unknown[12];
} prm_object_polygon_6_s00s01_t;

//After this one follows 'name_count' character strings:
typedef struct {
	unsigned char name[12];	/* Name */
} prm_object_polygon_6_name_t;

//Polygon type 0x06, subtype 0x80
typedef struct {
	unsigned short name_count;	/* Number of names */
	unsigned short unknown;
	prm_object_polygon_6_name_t *names;
} prm_object_polygon_6_s80_t;



//Polygon type 0x07 : Gouraud quad
typedef struct {
	unsigned short unknown1;
	unsigned short vertices[4];	/* Vertices index */
	unsigned long colours[4];
} prm_object_polygon_7_t;

//Polygon type 0x08 : Textured quad
typedef struct {
	WORD unknown1;
	WORD vertices[4];	/* Vertices index */
	WORD texture_index;	/* Texture index */
	BYTE name[4];
	WORD unknown2[5];
	DWORD texcoord[4];	/* Texture coordinates */	
} prm_object_polygon_8_t;

//Polygon type 0x0a,0x0b
typedef struct {
	unsigned short unknown[7];
} prm_object_polygon_10_t;

class PRM{
public:
	PRM(char *fn){
		infile = NULL;
		if(fn)
			infile = binopen(fn);
		if(infile)
			Read();
	}
	~PRM(){}

private:
	BINFILE *infile;
	prm_object_header_t header;
	std::vector<prm_object_polygon_1_t*> poly1;
	std::vector<prm_object_polygon_2_t*> poly2;
	std::vector<prm_object_polygon_3_t*> poly3;
	std::vector<prm_object_polygon_4_t*> poly4;
	std::vector<prm_object_polygon_5_t*> poly5;

	std::vector<prm_object_polygon_6_s00s01_t*> poly6s00s01;
	std::vector<prm_object_polygon_6_s80_t*> poly6s80;

	std::vector<prm_object_polygon_7_t*> poly7;
	std::vector<prm_object_polygon_8_t*> poly8;
	std::vector<prm_object_polygon_10_t*> polyA;
	std::vector<prm_object_polygon_10_t*> polyB;

	prm_object_vertex_t *verts;

	void Read(){
		binread(&header, sizeof(prm_object_header_t), 1, infile);
		header.vtx_count = readWORD(header.vtx_count);
		header.poly_count = readWORD(header.poly_count);
		verts = (prm_object_vertex_t*)calloc(header.vtx_count, 
			sizeof(prm_object_vertex_t));
		binread(verts, sizeof(prm_object_vertex_t), header.vtx_count, infile);

		for(int i = 0; i<header.poly_count-1;i++){
			WORD type;
			binread(&type, 2, 1, infile);
			type = readWORD(type);
			int test = sizeof(prm_object_polygon_8_t);
			void *work;
			switch(type){
			case 0x01:
				work = malloc(sizeof(prm_object_polygon_1_t));
				binread(work, sizeof( prm_object_polygon_1_t), 1, infile);
				poly1.push_back((prm_object_polygon_1_t*)work);
				break;
			case 0x02:
				work = malloc(sizeof(prm_object_polygon_2_t));
				binread(work, sizeof( prm_object_polygon_2_t), 1, infile);
				poly2.push_back((prm_object_polygon_2_t*)work);
				break;
			case 0x03:
				work = malloc(sizeof(prm_object_polygon_3_t));
				binread(work, sizeof( prm_object_polygon_3_t), 1, infile);
				poly3.push_back((prm_object_polygon_3_t*)work);
				break;
			case 0x04:
				work = malloc(sizeof(prm_object_polygon_4_t));
				binread(work, sizeof( prm_object_polygon_4_t), 1, infile);
				poly4.push_back((prm_object_polygon_4_t*)work);
				break;
			case 0x05:
				work = malloc(sizeof(prm_object_polygon_5_t));
				binread(work, sizeof( prm_object_polygon_5_t), 1, infile);
				poly5.push_back((prm_object_polygon_5_t*)work);
				break;
			case 0x06:
				WORD type6;
				binread(&type6, 2, 1, infile);
				type6 = readWORD(type6);
				switch(type6){
				case 0x00:
				case 0x01:
					work = malloc(sizeof(prm_object_polygon_6_s00s01_t));
					binread(work, sizeof(prm_object_polygon_6_s00s01_t), 1, infile);
					poly6s00s01.push_back((prm_object_polygon_6_s00s01_t*)work);
					break;
				case 0x80:
					work = malloc(sizeof(prm_object_polygon_6_s80_t));
					binread(work, sizeof(prm_object_polygon_6_s80_t)-4, 1, infile);
					prm_object_polygon_6_s80_t*temp=(prm_object_polygon_6_s80_t*)work;
					temp->names=(prm_object_polygon_6_name_t*)calloc(sizeof(prm_object_polygon_6_name_t), temp->name_count);
					binread(temp->names, temp->name_count, 12, infile);
					poly6s80.push_back((prm_object_polygon_6_s80_t*)work);
					break;
				}
				break;
			case 0x07:
				work = malloc(sizeof(prm_object_polygon_7_t));
				binread(work, sizeof( prm_object_polygon_7_t), 1, infile);
				poly7.push_back((prm_object_polygon_7_t*)work);
				break;
			case 0x08:
				work = malloc(sizeof(prm_object_polygon_8_t));
				binread(work, sizeof( prm_object_polygon_8_t), 1, infile);
				work = malloc(0x2a);
				binread(work, 0x2a, 1, infile);
				poly8.push_back((prm_object_polygon_8_t*)work);
				break;

			case 0x0A:
				work = malloc(sizeof(prm_object_polygon_10_t));
				binread(work, sizeof( prm_object_polygon_10_t), 1, infile);
				polyA.push_back((prm_object_polygon_10_t*)work);
				break;
			case 0x0B:
				work = malloc(sizeof(prm_object_polygon_10_t));
				binread(work, sizeof( prm_object_polygon_10_t), 1, infile);
				polyB.push_back((prm_object_polygon_10_t*)work);
				break;

			default:
				break;
			};
		}
	}
};

#endif