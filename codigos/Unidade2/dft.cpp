#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

#define RADIUS 20
using namespace cv;
using namespace std;

int deltaLSlider = 6;
int valorMaxDoDeltaHLoSlider = 100;

int delta0Slider = 100;
int valorMaxDoDelta0LoSlider = 100;

int valorCNoSlider = 1;
int valorMaxDoCNoSlider = 100;

int deltaHSlider = 42;
int valorMaxDoDeltaHNoSlider = 100;

char nomeDaBarra[50];
Mat filter, tmp;
int dft_N, dft_M;

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat tmpLocal, A, B, C, D;
  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;
  // reorganiza os quadrantes da transformada
  // A B-> D C
  // C D B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));
  // A <-> D
  A.copyTo(tmpLocal); D.copyTo(A); tmpLocal.copyTo(D);
  // C <-> B
  C.copyTo(tmpLocal); B.copyTo(C); tmpLocal.copyTo(B);
}

void modificaVariaveisDoFiltroBarrasNaImagem(){
	tmp = Mat(dft_M, dft_N, CV_32F);
	float delta0, deltaH, deltaL;
	delta0 = delta0Slider/10;
	deltaH = deltaHSlider/10;
	deltaL = deltaLSlider/10;
	for(int i=0; i<dft_M ;i++)
		for(int j=0; j<dft_N ;j++){
				tmp.at<float>(i,j) = (deltaH-deltaL)*
				(1.0-exp(-1*valorCNoSlider*(((i-dft_M/2)*(i-dft_M/2)
				+ ((j-dft_N/2)*(j-dft_N/2)))
					/(delta0*delta0))))+ deltaL;
			}
  Mat matrizQueRecece[]= {tmp, tmp};
  merge(matrizQueRecece, 2, filter);
	imshow("FiltroHomomorfo.png",tmp);
}

int main(int argc, char** argv){
  Mat padded, complexImage, image;
  Mat_<float> realInput, zeros;
  vector<Mat> planos;
  // habilita/desabilita ruido
  int noise=0;
  // frequencia do ruido
  int freq=10;
  // ganho inicial do ruido
  float gain=1;
  // valor do ruido
  float mean;
  // guarda tecla capturada
  char key;
  // valores ideais dos tamanhos da imagem
  // para calculo da DFT
  if(argc != 2)
  return -1;
  //coloca a imagem passada para escala de cinza
  image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);
  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
  dft_M - image.rows, 0,
  dft_N - image.cols,
  BORDER_CONSTANT, Scalar::all(0));
  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());
  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));
	modificaVariaveisDoFiltroBarrasNaImagem();
  // a função de transferência (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

  for(;;){
    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);
    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);
    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);
    // calcula o dft
    dft(complexImage, complexImage);
    // realiza a troca de quadrantes
    deslocaDFT(complexImage);
    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);
    // realiza a troca de quadrantes
    deslocaDFT(complexImage);
    // calcula o dft
    idft(complexImage, complexImage);
    // limpa o array de planos
    planos.clear();

    split(complexImage, planos);
    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("Imagem de saida", planos[0]);
    key = (char) waitKey(10);
		// para quando o esc é pressionado
    if( key == 27 ) break;

		sprintf( nomeDaBarra, "Delta L ");
		createTrackbar( nomeDaBarra, "Imagem de saida",
		&deltaLSlider, valorMaxDoDeltaHLoSlider, NULL);

		sprintf( nomeDaBarra, "Delta H");
    createTrackbar( nomeDaBarra, "Imagem de saida",
    &deltaHSlider, valorMaxDoDeltaHNoSlider, NULL);

		sprintf( nomeDaBarra, "delta0 ");
		createTrackbar( nomeDaBarra, "Imagem de saida",
		&delta0Slider, valorMaxDoDelta0LoSlider, NULL);

		sprintf( nomeDaBarra, "C ");
    createTrackbar( nomeDaBarra, "Imagem de saida",
    &valorCNoSlider, valorMaxDoCNoSlider, NULL);

    modificaVariaveisDoFiltroBarrasNaImagem();
  }
  return 0;
}
