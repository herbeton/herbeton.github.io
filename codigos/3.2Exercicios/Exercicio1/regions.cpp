#include <iostream>
#include <cv.h>
#include <highgui.h>
//http://pt.stackoverflow.com/questions/17328/edi%C3%A7%C3%A3o-b%C3%A1sica-de-imagens-utilizando-opencv
using namespace cv;
using namespace std;

int main(int, char**){
  Mat imagem;
  Mat imagemRecortada;
  Mat imagemRecortadaNegativa;
  Rect recorte;

  cout << "Forneça o inicial p(x, y) para a região que você quer!" << endl;
  cout << "x: ";
  cin >> recorte.x;
  cout << "y: ";
  cin >> recorte.y;
  cout << "Forneça o comprimento e a altura de pixels que voce quer!" << endl;
  cout << "Comprimento: ";
  cin >> recorte.width;
  cout << "Altura: ";
  cin >> recorte.height;

  if(recorte.x < 0 || recorte.y < 0 || recorte.width < 0 || recorte.height < 0){
      cout << "Digite tudo de novo!" << "\n";
      return 0;
  }

  //Transformou a matriz em formato de matriz
  imagem = imread("bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);

  if(!imagem.data){//caso a imagem seja diferende de bolhas.png
    cout << "nao abriu bolhas.png" << endl;
  }

  imshow("Imagem original", imagem);
  waitKey();


  imagemRecortada = imagem(recorte);

  imshow("Imagem recortada", imagemRecortada);
  waitKey();

  imagemRecortadaNegativa = 255 - imagemRecortada;

  imshow("Imagem recortada negativa", imagemRecortadaNegativa);
  waitKey();

  imagemRecortadaNegativa.copyTo(imagem(recorte));

  imshow("Imagem final", imagem);
  waitKey();
  return 0;
}
