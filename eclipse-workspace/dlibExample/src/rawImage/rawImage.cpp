
#define  _CRT_SECURE_NO_WARNINGS


#include "rawImage.h"
#include <fstream>
#include <iostream>
#include <new>



dg::RawImage::RawImage(const char *file, uint16 w, uint16 h)
    : m_filename(file), m_width(w), m_height(h) {
  imgFile = fopen(m_filename.c_str(), "rb");
  if (imgFile == NULL)
  {
	  fputs("File does not exist.\n", stderr);
	  exit(1);
  }
  fseek(imgFile, 0, SEEK_END);
  uint32 bSize = ftell(imgFile);
  rewind(imgFile); //set position of stream to the beginning
  m_numElements = bSize / (m_width * m_height);
}

dg::RawImage::RawImage(const char *file,const char *cameraType)
    : m_filename(file) {
	if(std::strcmp(cameraType,"psa") == 0)
	{
		m_height=480;
		m_width=752;
	}
	if(std::strcmp(cameraType,"volvo") == 0)
	{
		m_height=800;
		m_width=1280;
	}

	imgFile = fopen(m_filename.c_str(), "rb");
	if (imgFile == NULL)
	{
		fputs("File does not exist.\n", stderr);
		exit(1);
	}
	fseek(imgFile, 0, SEEK_END);
	uint32 bSize = ftell(imgFile);
	rewind(imgFile); //set position of stream to the beginning
	m_numElements = bSize / (m_width * m_height);
}

dg::RawImage::~RawImage() { fclose(imgFile); }

void dg::RawImage::getFrameAt(uint32 index, uint8 *img) {
  //assert(index < m_numElements);

  size_t num = (m_width * m_height);
  seek(index);
  size_t result = fread(img, sizeof(uint8), num, imgFile);
  if (result != m_width * m_height)
  {
	  fputs("Can not read file.\n", stderr);
	  exit(3);
  }
}

void dg::RawImage::getFrameAt(uint32 index, dg::img_type& img) {
	size_t num = (m_width * m_height);
	img.set_size(m_height, m_width);
	seek(index);
	size_t result = fread(img.begin(), sizeof(uint8), num, imgFile);
	if (result != m_width * m_height)
	{
		fputs("Can not read file.\n", stderr);
		exit(3);
	}
}

void dg::RawImage::saveFrame(uint32 index,dg::img_type& img){

	std::stringstream fileName(std::string(m_filename),std::ios_base::out|std::ios_base::ate);
	fileName<<"_"<<index;
	std::ofstream outfile (fileName.str().c_str(),std::ofstream::binary);
	outfile.write((const char*)img.begin(),img.size());
	std::cout<<fileName.str().c_str()<<" was saved"<<std::endl;
	outfile.flush();
	outfile.close();

}
