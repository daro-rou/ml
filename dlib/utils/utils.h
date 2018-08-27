
#ifndef DLIB_SCAN_fHOG_UTILS_Hh_
#define DLIB_SCAN_fHOG_UTILS_Hh_

#include "../matrix.h"
#include "../image_transforms.h"
#include "../array.h"
#include "../array2d.h"
//#include "object_detector.h"

#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>

namespace utils{


template <
    typename T,
    typename mm1,
    typename mm2
    >
dlib::matrix<unsigned char> draw_fhog(
    const dlib::array<dlib::array2d<T,mm1>,mm2>& hog,
    const long cell_draw_size = 15,
    const float min_response_threshold = 0.0
);

class logFile{
    public:
    	logFile (std::string& filename):outfile(filename.c_str()){
    	}
       	logFile (const char* filename):outfile(filename){
        	}
    	logFile (std::string filename_, int x, int y){
    		std::stringstream fileName(filename_,std::ios_base::out|std::ios_base::ate);
    		fileName<<"_"<<x<<"x"<<y<<".R";
    		outfile.open(fileName.str().c_str());
    	}
    	~logFile(){
    		outfile.flush();
    		outfile.close();
    	}
        inline  operator std::ostream& ()
        {
            return outfile;
        }
    private:
    	std::ofstream outfile;
    };


    template <bool RScript>
    class print_fhog_as_csv_helper;

	class r_matrix_helper {
	public:
		r_matrix_helper(std::ostream& out_) :
				out(out_) {
		}

		template<typename T>
		r_matrix_helper& operator<<(const dlib::array2d<T>& m) {
			print_fhog_as_csv_helper<true>(out) << m;
			out << "), nrow = " << m.nr() << ", ncol = " << m.nc()
					<< ", byrow = TRUE)" << std::endl;
			return (*this);
		}

		r_matrix_helper& operator<<(std::string name_) {
			name=name_;
			return (*this);
		}

		template <

		        long num_rows,
		        long num_cols
		        >
		r_matrix_helper& operator<<(const dlib::matrix<float,num_rows,num_cols>& m) {

			out << name.c_str() << "=matrix( c(";
			for(long k =0;k<num_rows;++k)
				for(int l=0;l<num_cols;++l)
			     			if(k == (num_rows-1) && l == (num_cols-1))
			     				out<<m(k,l)<<" ";
			     			else
			     				out<<m(k,l)<<", ";
			 out<<std::endl;
			 out << "), nrow = " << num_rows << ", ncol = " << num_cols
			 					<< ", byrow = TRUE)" << std::endl;
			return (*this);
		}

		template <

		        long num_rows,
		        long num_cols
		        >
		r_matrix_helper& operator<<(
					const dlib::array2d< dlib::matrix<float,num_rows,num_cols> >& m
					)
		{
		     typedef typename dlib::array2d<dlib::matrix<float,num_rows,num_cols> >::row row_type;
		     std::string tmp_name=name;

		     for(int k =0;k<m.nr();++k)
		     {
		     	row_type row = m[k];
		     	for(int l=0;l<m.nc();++l)
		     	{
		     		std::stringstream ss_name(tmp_name,std::ios_base::out|std::ios_base::ate);
		     		ss_name<<"."<<k<<"."<<l;
		     		name=ss_name.str();
		     		(*this)<<row[l];
		     	}
		     }
		     name=tmp_name;
		     return (*this);
		 }

	private:
		std::ostream& out;
		std::string name;
	};

    class r_matrix_type {};
    const r_matrix_type r_matrix      = r_matrix_type();
    inline r_matrix_helper operator<< (
        std::ostream& out,
        const r_matrix_type&
    )
    {
        return r_matrix_helper(out);
    }

    template <bool RScript>
    class print_fhog_as_csv_helper
    {
        /*!

            In particular, this code allows you to write statements like:
               std::cout<<fhog_csv<<feats;
            and have it print with commas separating each element.
        !*/
    public:
    	print_fhog_as_csv_helper (std::ostream& out_) : out(out_) {}

        template <typename T>
        std::ostream& operator<< (
            const dlib::array<dlib::array2d<T> >& feats
        );

        template <typename T>
        std::ostream& operator<< (
            const dlib::array2d<T>& m);


        print_fhog_as_csv_helper<RScript>& operator<< (
        	const std::string name_
			)
        {
        	name=name_;
        	return (*this);
        }
    private:
        std::ostream& out;
        mutable std::string name;
    };

    template<>
    template <typename T>
     std::ostream& print_fhog_as_csv_helper<true>::operator<< (
         const dlib::array<dlib::array2d<T> >& feats
     )
     {
      	for(int i=0;i<feats.size();++i)
      	{
      		out<<name.c_str()<<"."<<i<<"=matrix( c(";
      		(*this)<<feats[i];
      		out<<"), nrow = "<<feats[i].nr()<<", ncol = "<< feats[i].nc() <<", byrow = TRUE)"<<std::endl;
      	}
      	return out;
     }

    template<>
    template <typename T>
    std::ostream& print_fhog_as_csv_helper<true>::operator<< (
        const dlib::array2d<T>& m
     )
     {
     	typedef typename dlib::array2d<T>::row row_type;
     	//std::cout<<"feats.size()="<<feats.size() << std::endl;
     	for(int k =0;k<m.nr();++k)
     	{
     		row_type row = m[k];
     		for(int l=0;l<m.nc();++l)
     			if(k == (m.nr()-1) && l == (m.nc()-1))
     				out<<row[l]<<" ";
     			else
     				out<<row[l]<<", ";
 			out<<std::endl;
     	}
         return out;
     }

    template<>
    template <>
    std::ostream& print_fhog_as_csv_helper<true>::operator<< (
        const dlib::array2d<unsigned char>& m
     )
     {
     	typedef typename dlib::array2d<unsigned char>::row row_type;
     	//std::cout<<"feats.size()="<<feats.size() << std::endl;
     	for(int k =0;k<m.nr();++k)
     	{
     		row_type row = m[k];
     		for(int l=0;l<m.nc();++l)
     		{
     			unsigned char u8= row[l];
     			unsigned int u16 =(unsigned int) u8;
     			out<<u16;
     			if(k == (m.nr()-1) && l == (m.nc()-1))
     				out<<" ";
     			else
     				out<<", ";
     		}
 			out<<std::endl;
     	}
         return out;
     }

    template<>
    template <typename T>
    std::ostream& print_fhog_as_csv_helper<false>::operator<< (
         const dlib::array<dlib::array2d<T> >& feats
     )
     {
     	std::cout<<"feats.size()="<<feats.size() << std::endl;
     	for(int i=0;i<feats.size();++i)
     	{
     		std::cout<<"feats["<<i<<"]="<<feats[i].nc()<<"x"<<feats[i].nr()<< std::endl;
        }
         return out;
     }

    template<>
    template <typename T>
    std::ostream& print_fhog_as_csv_helper<false>::operator<< (
        const dlib::array2d<T>& m
     )
     {}

    class print_fhog_short {};
    const print_fhog_short fhog_info      = print_fhog_short();
    inline print_fhog_as_csv_helper<false> operator<< (
        std::ostream& out,
        const print_fhog_short&
    )
    {
        return print_fhog_as_csv_helper<false>(out);
    }

    class print_fhog_verbose {};
    const print_fhog_verbose fhog_csv      = print_fhog_verbose();
    inline print_fhog_as_csv_helper<true> operator<< (
        std::ostream& out,
        const print_fhog_verbose&
    )
    {
        return print_fhog_as_csv_helper<true>(out);
    }



    template <
        //typename pyramid_type,
        typename image_type
       ,typename feature_extractor_type
        >
    void show_image_ (
    	 dlib::image_window& win
        ,const image_type& img
        ,const feature_extractor_type& fe
		,dlib::array<dlib::array2d<float> >& feats
		,int cell_size
		,int filter_rows_padding
		,int filter_cols_padding

    )
    {
        win.clear_overlay();
        win.set_image(img);
//        dlib::image_window winhog(draw_fhog(feats,7));
        std::cout<<"Image size: "<<img.nc()<<"x"<<img.nr()<<std::endl;
        std::cout<<"cell_size: "<<cell_size<<std::endl;
        std::cout<<"filter_rows_padding: "<<filter_rows_padding<<std::endl;
        std::cout<<"filter_cols_padding: "<<filter_cols_padding<<std::endl;
        logFile rScript("fhog",img.nc(),img.nr());
        rScript<<fhog_csv<<"feature"<<feats;
        rScript<<r_matrix<<"img"<<img;
        std::cout<<fhog_info<<feats;
        //std::cout<<"Press enter to continue.."<<std::endl;
        //std::cin.get();

    }
}//namespace
#endif
