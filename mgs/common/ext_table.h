#pragma once

struct EXT_TABLE  {
	const char* name;
	int id;
};

const EXT_TABLE ext_table[77] = {
	{"rdv",  0x22},
	{"mtfa", 0x0A},
	{"mds",  0x05},
	{"raw",  0x6E},
	{"dlz",  0x21},
	{"rat",  0x6B},
	{"far",  0x0A},
	{"at3",  0x4F},
	{"vab",  0x1D},
	{"rvb",  0x1F},
	{"eqpp", 0x19},
	{"csr",  0x67},
	{"psq",  0xFF},
	{"gcx",  0x02},
	{"cvd",  0x10},
	{"ico",  0x5E},
	{"gsp",  0x20},
	{"txn",  0x03},
	{"nv2",  0x15},
	{"sds",  0x1C},
	{"fpo",  0x62},
	{"rpd",  0x13},
	{"dld",  0x21},
	{"vlm",  0x65},
	{"var",  0x68},
	{"phs",  0x18},
	{"pdl",  0x29},
	{"ptl",  0x2A},
	{"bpef", 0x27},
	{"spu",  0x16},
	{"la2",  0x5F},
	{"dbd",  0x5C},
	{"octl", 0x24},
	{"rcm",  0x6C},
	{"ola",  0x6D},
	{"lt2",  0x06},
	{"mtsq", 0x09},
	{"vfp",  0x25},
	{"row",  0x6E},
	{"mdh",  0x04},
	{"fcv",  0x17},
	{"bin",  0x01},
	{"cnp",  0x11},
	{"lt3",  0x06},
	{"img",  0x69},
	{"octt", 0x23},
	{"vib",  0x6A},
	{"zon",  0x12},
	{"octs", 0x26},
	{"nav",  0x0F},
	{"png",  0x5A},
	{"mdn",  0x0D},
	{"pam",  0x5B},
	{"abc",  0x14},
	{"tri",  0x03},
	{"cv4",  0x63},
	{"cpef", 0x2B},
	{"mtra", 0x6F},
	{"geom", 0x0C},
	{"phpr", 0x1A},
	{"dlp",  0x2C},
	{"sfp",  0x28},
	{"vpo",  0x61},
	{"cv2",  0x7},
	{"mcl",  0x64},
	{"lh4",  0x66},
	{"mtar", 0x08},
	{"jpg",  0x5D},
	{"eft",  0x11},
	{"slot", 0x60},
	{"ssp",  0x1E},
	{"mdl",  0x0D},
	{"mtcm", 0x0B},
	{"phes", 0x1B},
	{"mdb",  0x0D},
	{"van",  0x10},
	{"mdc",  0x0D}
};

inline
const char* getExtForID(uint8_t id) {
	for (int i = 0; i < 77; i++) {
		if (ext_table[i].id == id)
			return ext_table[i].name;
	}
	return "";
}

inline
uint8_t getIDforExt(const char* ext) {
	for (int i = 0; i < 77; i++) {
		if (!strcmp(ext_table[i].name, ext))
			return ext_table[i].id;
	}
	return -1;
}

