#ifndef __RAW_IMAGE__
#define __RAW_IMAGE__
#include "glob_type.h"
#include <ios>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include "dlib/array2d.h"

namespace dg {

	typedef bool boolean_t;
	typedef dlib::array2d<unsigned char> img_type;
/**
* Read raw file of one or more frames of a recording.
*/
class RawImage {

public:
  /**
  * Reads all frames
  *
  * @param file name of raw file
  * @param w width of frame
  * @param h height of frame
  */
	RawImage(const char *file, uint16 w = 752, uint16 h = 480);
	RawImage(const char *file,const char *cameraType);

  ~RawImage();

  /**
  * Get a specific frame.
  *
  * @param index position of specific frame
  * @param img buffer to save the frame
  */
  void getFrameAt(uint32 index, uint8 *img);
  void getFrameAt(uint32 index, img_type& img);

  void saveFrame(uint32 index, img_type& img);
  uint32 getNumOfFrames() const { return m_numElements; }
  uint16 getHeight() const { return m_height; }
  uint16 getWidth() const { return m_width; }
  

private:
  class wrongIndex: public std::exception
  {
  public:
	  virtual const char* what(){
		  return "index out of range";
	  }
  };
  void seek(uint32 index)
  {
	  if (index<1 || index> m_numElements)
		  throw wrongIndex();
	  long int pos =(index-1)*m_height*m_width;
	  fseek(imgFile, pos, SEEK_SET);
  }
  /**
  * Read raw file
  */
  boolean_t readRawFile();

  std::string m_filename;
  uint16 m_width;
  uint16 m_height;
  
  FILE *imgFile;
	
  uint32 m_numElements;


	
};
}
#endif //__RAW_IMAGE__
