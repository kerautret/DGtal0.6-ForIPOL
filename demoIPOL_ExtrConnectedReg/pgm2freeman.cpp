#include "DGtal/io/colormaps/GrayscaleColorMap.h"
#include "DGtal/io/readers/PNMReader.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "DGtal/images/ImageSelector.h"

#include "DGtal/geometry/curves/FreemanChain.h"
#include "DGtal/geometry/helpers/ContourHelper.h"
#include "DGtal/topology/helpers/Surfaces.h"

#include <vector>
#include <string>

#include "ImaGene/Arguments.h"

using namespace DGtal;

static ImaGene::Arguments args;


void saveAllContoursAsFc(std::vector< std::vector< Z2i::Point >  >  vectContoursBdryPointels, unsigned int minSize){
  for(unsigned int k=0; k<vectContoursBdryPointels.size(); k++){
    if(vectContoursBdryPointels.at(k).size()>minSize){
      FreemanChain<Z2i::Integer> fc (vectContoursBdryPointels.at(k));    
      cout << fc.x0 << " " << fc.y0   << " " << fc.chain << endl; 
	  
    }
  }
}


void saveSelContoursAsFC(std::vector< std::vector< Z2i::Point >  >  vectContoursBdryPointels, 
			 unsigned int minSize, Z2i::Point refPoint, double selectDistanceMax){
  for(unsigned int k=0; k<vectContoursBdryPointels.size(); k++){
    if(vectContoursBdryPointels.at(k).size()>minSize){
      Z2i::Point ptMean = ContourHelper::getMeanPoint(vectContoursBdryPointels.at(k));
      unsigned int distance = (unsigned int)ceil(sqrt((double)(ptMean[0]-refPoint[0])*(ptMean[0]-refPoint[0])+
						      (ptMean[1]-refPoint[1])*(ptMean[1]-refPoint[1])));
      if(distance<=selectDistanceMax){
	FreemanChain<Z2i::Integer> fc (vectContoursBdryPointels.at(k));    
	cout << fc.x0 << " " << fc.y0   << " " << fc.chain << endl; 
      }      
    }    
  }
}




int main( int argc, char** argv )
{

  args.addIOArgs(  false, false );  
  
  args.addBooleanOption( "-h", "-h: display this message." );
  args.addOption("-image",  "-image: set the input image filename ", "image.pgm"); 
  args.addOption( "-badj", "-badj <0/1>: 0 is interior bel adjacency, 1 is exterior (def. is 0).", "0" );
  args.addOption( "-minThreshold", "-minThreshold <val>: minimal threshold value for binarizing PGM gray values (def. is 128).", "0" );
  args.addOption( "-maxThreshold", "-maxThreshold <val>: maximal threshold value for binarizing PGM gray values (def. is 255).", "128" );
  args.addOption("-thresholdRange", "-thresholdRange <min> <incr> <max> use a range interval as threshold: for each possible i, it define a digital sets [min, min+((i+1)*increment)] such that min+((i+1)*increment)< max  and extract their boundary.","0", "10", "255");
  args.addOption( "-min_size", "-min_size <m>: minimum digital length of contours for output (def. is 4).", "4" );
  
  args.addOption("-selectContour", "-selectContour <x0> <y0> <distanceMax>: select the contours for which the first point is near (x0, y0) with a distance less than <distanceMax>","0", "0", "0" );
  args.addBooleanOption("-invertVerticalAxis", "-invertVerticalAxis used to transform the contour representation (need for DGtal), used o nly for the contour displayed, not for the contour selection (-selectContour). ");
      
    
    
  if ( ( argc <= 1 ) ||  ! args.readArguments( argc, argv ) ) 
    {
      cerr << args.usage( "pgm2freeman: ", 
			  "Extracts all 2D contours from a PGM image given on the standard input and writes them on the standard output as FreemanChain's. \nTypical use: \n pgm2freeman -threshold 200 -image image.pgm > imageContour.fc ",
			  "" )
	   << endl;
      return 1;
    }  
 
  
 
 
  int minThreshold = args.getOption("-minThreshold")->getIntValue(0);
  int maxThreshold = args.getOption("-maxThreshold")->getIntValue(0);
  unsigned int minSize = args.getOption("-min_size")->getIntValue(0);

 
  bool select=false;
  bool thresholdRange= args.check("-thresholdRange");
 
  int min, max, increment;
  if(thresholdRange){
    minThreshold = args.getOption("-thresholdRange")->getIntValue(0);
    increment = args.getOption("-thresholdRange")->getIntValue(1);
    maxThreshold = args.getOption("-thresholdRange")->getIntValue(2);
  }
 
  Z2i::Point selectCenter;
  unsigned int selectDistanceMax = 0; 
 

  if(args.check("-selectContour")){
    select=true;   
    selectCenter[0] = args.getOption("-selectContour")->getIntValue(0);
    selectCenter[1] = args.getOption("-selectContour")->getIntValue(1);
    selectDistanceMax = args.getOption("-selectContour")->getIntValue(2);
  }
 
  typedef ImageSelector < Z2i::Domain, unsigned char>::Type Image;
  typedef IntervalThresholder<Image::Value> Binarizer; 
  string imageFileName = args.getOption("-image")->getValue(0);
  Image image = PNMReader<Image>::importPGM( imageFileName ); 
  
  Z2i::KSpace ks;
  if(! ks.init( image.domain().lowerBound(), 
		image.domain().upperBound(), true )){
    trace.error() << "Problem in KSpace initialisation"<< endl;
  }
  
  bool badj = (args.getOption("-badj")->getIntValue(0))!=1;
  
  
  if (!thresholdRange){
    Binarizer b(minThreshold, maxThreshold); 
    PointFunctorPredicate<Image,Binarizer> predicate(image, b); 
    trace.info() << "DGtal contour extraction from thresholds ["<<  minThreshold << "," << maxThreshold << "]" ;
    
    SurfelAdjacency<2> sAdj( badj );
    std::vector< std::vector< Z2i::Point >  >  vectContoursBdryPointels;
    Surfaces<Z2i::KSpace>::extractAllPointContours4C( vectContoursBdryPointels,
  						      ks, predicate, sAdj );  
    if(select){
      saveSelContoursAsFC(vectContoursBdryPointels,  minSize, selectCenter,  selectDistanceMax);
    }else{
      saveAllContoursAsFc(vectContoursBdryPointels,  minSize); 
    }
    trace.info()<< " [done] " << endl;
  }else{
    for(int i=0; minThreshold+(i+1)*increment< maxThreshold; i++){
      min = minThreshold;
      max = minThreshold+(i+1)*increment;
      
      Binarizer b(min, max); 
      PointFunctorPredicate<Image,Binarizer> predicate(image, b); 
      
      trace.info() << "DGtal contour extraction from thresholds ["<<  min << "," << max << "]" ;
      SurfelAdjacency<2> sAdj( badj );
      std::vector< std::vector< Z2i::Point >  >  vectContoursBdryPointels;
      Surfaces<Z2i::KSpace>::extractAllPointContours4C( vectContoursBdryPointels,
  							ks, predicate, sAdj );  
      if(select){
  	saveSelContoursAsFC(vectContoursBdryPointels,  minSize, selectCenter,  selectDistanceMax);
      }else{
  	saveAllContoursAsFc(vectContoursBdryPointels,  minSize); 
      }
      trace.info() << " [done]" << endl;
    }
  }
  return 0;
}

