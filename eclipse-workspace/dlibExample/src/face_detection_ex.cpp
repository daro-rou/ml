// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to find frontal human faces in an image.  In
    particular, this program shows how you can take a list of images from the
    command line and display each on the screen with red boxes overlaid on each
    human face.

    The examples/faces folder contains some jpg images of people.  You can run
    this program on them and see the detections by executing the following command:
        ./face_detection_ex faces/*.jpg


    This face detector is made using the now classic Histogram of Oriented
    Gradients (HOG) feature combined with a linear classifier, an image pyramid,
    and sliding window detection scheme.  This type of object detector is fairly
    general and capable of detecting many types of semi-rigid objects in
    addition to human faces.  Therefore, if you are interested in making your
    own object detectors then read the fhog_object_detector_ex.cpp example
    program.  It shows how to use the machine learning tools which were used to
    create dlib's face detector.


    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most cu
	rrent machines.
*/


#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include "rawImage/rawImage.h"
#include <iostream>

using namespace dlib;
using namespace std;
#define SHOW_IMG	1
const char pShapePredictor[] = "/home/dan/eclipse-workspace/dlibExample/shape_predictor_68_face_landmarks.dat";
/**
**You can get the shape_predictor_68_face_landmarks.dat file from:
** "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2"
*/

// ----------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    try
    {
        if (argc < 3)
        {
            cout << "Give a raw file as arguments to this program and volvo or psa as a second argument." << endl;
            return 0;
        }
		dg::RawImage movie(argv[1], argv[2]);
		// We need a face detector.  We will use this to get bounding boxes for
		// each face in an image.
		frontal_face_detector detector = get_frontal_face_detector();
		// And we also need a shape_predictor.  This is the tool that will predict face
		// landmark positions given an image and face bounding box.  Here we are just
		// loading the model from the shape_predictor_68_face_landmarks.dat file you gave
		// as a command line argument.
		shape_predictor sp;
		deserialize(pShapePredictor) >> sp;
        //image_window win;
#if defined (SHOW_FHOG_FEATURE)
        image_window hogwin(draw_fhog(detector), " fHOG detector");
#endif

#if defined (SHOW_IMG)
		image_window win;
#endif

		dg::img_type img;
		char cmd;
		unsigned int i = 1;
        //for (unsigned int i = 1; i < movie.getNumOfFrames(); ++i)
		while(i <= movie.getNumOfFrames())
        {
            cout << "processing frame: " << i << endl;
			movie.getFrameAt(i, img);
            //array2d<unsigned char> img;
            //load_image(img, argv[i]);
			// Make the image bigger by a factor of two.  This is useful since
			// the face detector looks for faces that are about 80 by 80 pixels
			// or larger.  Therefore, if you want to find faces that are smaller
			// than that then you need to upsample the image as we do here by
			// calling pyramid_up().  So this will allow it to detect faces that
			// are at least 40 by 40 pixels in size.  We could call pyramid_up()
			// again to find even smaller faces, but note that every time we
			// upsample the image we make the detector run slower since it must
			// process a larger image.
			// pyramid_up(img);
#if defined (SHOW_FHOG_FEATURE)
			//fhog fetature
			array2d<matrix<float, 31, 1> > fhog;
			//extract the fhog feature
			extract_fhog_features(img, fhog);
			cout << "fhog image has " << fhog.nr() << " rows and " << fhog.nc() << " columns." << endl;
			image_window winhog(draw_fhog(fhog));
#endif
            // Now tell the face detector to give us a list of bounding boxes
            // around all the faces it can find in the image.
            std::vector<rectangle> dets = detector(img);
            cout << "Number of faces detected: " << dets.size() << endl;
            for(int i=0;i<dets.size();++i){
            	cout<<"left="<<dets[i].left()<<", bottom="<<dets[i].bottom();
            	cout<<", right="<<dets[i].right()<<", top="<<dets[i].top()<<endl;
            }

			// Now we will go ask the shape_predictor to tell us the pose of
			// each face we detected.
			std::vector<full_object_detection> shapes;
			for (unsigned long j = 0; j < dets.size(); ++j)
			{
				full_object_detection shape = sp(img, dets[j]);
				cout << "number of parts: " << shape.num_parts() << endl;
				cout << "pixel position of first part:  " << shape.part(0) << endl;

				cout << "pixel position of second part: " << shape.part(1) << endl;
				// You get the idea, you can get all the face part locations if
				// you want them.  Here we just store them in shapes so we can
				// put them on the screen.
				shapes.push_back(shape);
			}
            // Now we show the image on the screen and the face detections as
            // red overlay boxes.
#if defined (SHOW_IMG)
			win.clear_overlay();
            win.set_image(img);
            win.add_overlay(dets, rgb_pixel(255,0,0));
			// Now let's view our face poses on the screen.
			win.add_overlay(render_face_detections(shapes));
#endif
			cout << "Hit enter to process the next image, b+<enter>: back, q+<enter>: quit"
					<< endl;
			cmd = 0;
			cin.get(cmd);
			if ('b' == cmd) {
				cin.get();
				if (i > 1)
					--i;
			} else if ('q' == cmd) {
				break;
			} else if('g'==cmd){
            	cin>>i;
            	cin.get();
            }else if('s'==cmd){
            	cin.get();
            	movie.saveFrame(i, img);;
            }else {
				++i;
			}

		}
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
    cout<<"Good-bye cruel world!!"<<endl;
}

// ----------------------------------------------------------------------------------------



