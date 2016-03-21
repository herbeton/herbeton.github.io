#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects,nobjectsTotal = 0;
  int nobjectsComBuracos = 0, nobjectsSemBuracos = 0;

  CvPoint p;//ponto 2d
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  p.x=0;
  p.y=0;

  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  //Aplicando 255 para todos os pontos que existem na
  //borda da imágem
  for (int i = 0; i < width; i++) {
    image.at<uchar>(i, height - 1) = 255;
    image.at<uchar>(i, 0) = 255;
  }

  for (int i = 0; i < height; i++) {
    image.at<uchar>(0, i) = 255;
    image.at<uchar>(width - 1, i) = 255;
  }

  //aplicando floodFill para retirar os elementos
  //brancos das bordas
  floodFill(image, p, 0);

  //aplicando o floodFill no fundo da imagem
  floodFill(image, p, 3);


  // busca objetos com buracos presentes
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
        nobjects++;
        if(nobjects == 255){
          nobjectsTotal = nobjectsTotal + nobjects;
          nobjects = 0;
        }
    		p.x=j;
    		p.y=i;
        //sem bolha rotulado com 77
    		floodFill(image,p,77);
	  }
	}
  }
  //procura por objetos que tem buracos
  //177 são os objetosComBuracos e o 77 são os objetos sem buracos
  nobjectsComBuracos = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if(image.at<uchar>(i,j) == 77 || image.at<uchar>(i,j) == 177){
        p.x = j;
        p.y = i;
      }
      else if(image.at<uchar>(i, j) == 0){
        if(image.at<uchar>(p.y,p.x) == 77){
          floodFill(image, p, 177);
          nobjectsComBuracos++;
        }
        p.x = j;
        p.y = i;
        floodFill(image, p, 177);
      }
    }
  }

  //procurando por objetos com bolhas

  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  nobjectsSemBuracos = nobjects - nobjectsComBuracos;
  std::cout << "Objetos com buracos: " << nobjectsComBuracos << std::endl;
  std::cout << "Objetos sem buracos: " << nobjectsSemBuracos << std::endl;
  return 0;
}
