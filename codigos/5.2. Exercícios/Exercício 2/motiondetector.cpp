#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int seuil = 10;
bool entrou = false;
void Compare(Mat f, Mat oldF)
{
    if (oldF.rows == 0){
      return;
    }
    int contPixelsBulidos = 0;
    for (int i = 0; i < f.rows; i++)
    {
        for (int j = 0; j < f.cols; j++)
        {
            if (abs(f.at<cv::Vec3b>(i, j)[2] - oldF.at<cv::Vec3b>(i, j)[2]) >seuil &&
                abs(f.at<cv::Vec3b>(i, j)[0] - oldF.at<cv::Vec3b>(i, j)[0]) >seuil&&
                abs(f.at<cv::Vec3b>(i, j)[1] - oldF.at<cv::Vec3b>(i, j)[1]) > seuil)
            {
                f.at<cv::Vec3b>(i, j)[2] = 255;
                f.at<cv::Vec3b>(i, j)[1] = 255;
                f.at<cv::Vec3b>(i, j)[0] = 0;
                contPixelsBulidos++;
            }
        }
        //std::cout << (f.rows * f.cols) << std::endl;
    }
    if(contPixelsBulidos/ (f.cols) > 48){
      entrou = true;
    }
}

int main(int argc, char** argv){
  Mat image, imageTemp, oldImage;
  int width, height;
  VideoCapture cap;//serve como uma declaração para os frams do video
  vector<Mat> planes;//planes[0], planes[1] e planes[2] armazenarão as componentes de cor Vermelho, Verde e Azul, respectivamente.
  Mat histR, histG, histB;// gera o histograma referente a cor da imagem capturada
  int nbins = 64;//tamanho do vetor utilizado para armazenar o histograma
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);//"abrindo a camera" para a captura

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));

  while(1){
    cap >> image;

    if (!cap.grab()) continue;
    //else{std::cout << "Bla" << std::endl;}

    image.copyTo(imageTemp);
    Compare(image, oldImage);
    imageTemp.copyTo(oldImage);

    if(entrou){
      putText( image, "Pare!", Point(height/2,width/2), CV_FONT_HERSHEY_COMPLEX, 2,
           Scalar(255, 255, 0), 5, 0 );
    }

    split (image, planes);
    //o calcHist calcula os componentes de cor do histograma
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    //Normalização do histograma entre o valor máximo e o valor mínimo dos componentes de cor
    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    //inicia os histogramas com preto(0)
    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    //parte da criação dos histogramas em forma de linha com a função line()
    for(int i=0; i<nbins; i++){
      line(histImgR, Point(i, histh),
           Point(i, cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG, Point(i, histh),
           Point(i, cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB, Point(i, histh),
           Point(i, cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }

    //passa para a parte superior direita da imagem da camera
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh   ,nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh ,nbins, histh)));

    imshow("image", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
