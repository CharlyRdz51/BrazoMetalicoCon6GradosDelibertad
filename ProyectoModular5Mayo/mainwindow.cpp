#include<opencv4/opencv2/cvconfig.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<opencv2/core/core.hpp>
#include<opencv2/ml/ml.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/video/background_segm.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/imgcodecs.hpp>
#include"mat2qimage.h"
#include<QTimer>
#include<QDebug>
#include<QtSerialPort>
#include<QSerialPortInfo>
#include<QFile>
#include<QTextStream>
#include<QDateTime>
#include<QDate>
#include<QFileDialog>
#include<QDebug>
#include<QtNetwork>
#include<QMouseEvent>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;


VideoCapture camara(0);
//VideoCapture camara("http://192.168.43.1:8080/video");
Mat salidaMux;
Mat imagenProcesada;
//Mat nombreMatriz(Y, X, TipoMatriz, Scalar(B,G,R));
Mat error33(600, 800, CV_8UC3, Scalar(237,82,170));
Mat imagenFija;
Mat imagenChica;

int contadorImagenes = 0;
bool temporizadorOn = true;
int contadorTiempo = 0;

QString MainWindow::conexionWeb(QString url, QString mensaje){
    QString respuesta = "error 1";

     QNetworkInterface redConectada = QNetworkInterface::interfaceFromName("wlo1");
    QList<QNetworkAddressEntry> lista = redConectada.addressEntries();
    //Comprobacion de que la red este activa
    if(!lista.empty()){
        QNetworkAddressEntry IP = lista.first();
        qDebug() << "La conexion de red esta activa: " << IP.ip() << endl;
        //Crear el mensaje HTML/HTTP
        QByteArray longitudMensaje = QByteArray::number(mensaje.size());
        QNetworkRequest solicitud;
        QNetworkAccessManager *clienteWeb = new QNetworkAccessManager();
        QUrl servidor(url.toUtf8().constData());
        //Comprobar que la direccion url sea valida
        if(servidor.isValid()){
            qDebug() << "El servidor se encuentra activo" << endl;
            //Formar el mensaje HTTP
            solicitud.setUrl(servidor);
            solicitud.setRawHeader(QByteArray("User-Agent"),QByteArray("bot"));
            solicitud.setRawHeader(QByteArray("Connection"),QByteArray("close"));
            solicitud.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
            solicitud.setHeader(QNetworkRequest::ContentLengthHeader, longitudMensaje);
            //Realizar la conexion
            QNetworkReply *conexionServidor = clienteWeb->post(solicitud, mensaje.toLatin1());
            //QNetworkReply *conexionServidor = clienteWeb->get(QNetworkRequest(servidor));
            //Esperar a que servidor responda
            QEventLoop funcionLoop;
            QObject::connect(clienteWeb, SIGNAL(finished(QNetworkReply*)), &funcionLoop, SLOT(quit()));
            funcionLoop.exec();
            //Leer la respuesta del servidor
            char datosWeb[300000];
            respuesta.clear();
            int sv=conexionServidor->read(datosWeb,300000);
            for(int i=0;i<sv;i++) {
                respuesta+= datosWeb[i];
             }
        }else{
            respuesta = "Error 3" ;
             }
        }else{
        respuesta = "Error 2";
             }
    return respuesta;
             }

  void MainWindow::Temporizador(){

  if(temporizadorOn){
      contadorTiempo++;
      Mat imagen;
      contadorImagenes++;
      if(ui->radioButton_5->isChecked()){
          //Capturar la imagen original que viene de la camara
          camara >> imagen;
      }else{
          if(imagenFija.empty()){
              error33.copyTo(imagen);
          }else{
              imagenFija.copyTo(imagen);
          }
          }
      if(!imagen.empty()){
          cv::resize(imagen, imagenChica, Size(512,282),0,0,0);
          }

      if(ui->radioButton->isChecked()){
          imagenChica.copyTo(imagenProcesada);

      }

            else{
                error33.copyTo(imagenProcesada);
            }

            if(!imagenProcesada.empty()){
                //Paso · 2 - Cambiar el tamaño
                cv::resize(imagenProcesada, salidaMux, Size(512,288),0,0,INTER_LINEAR);
            }

            //Convertir la imagen Mat a una imagen de QT
            QImage imagenQT = Mat2QImage(salidaMux);

            //Convertir la imagen de QT, a un mapa de pixeles
            QPixmap mapaPixeles = QPixmap::fromImage(imagenQT);
            //Borra todo lo que este en la etiqueta "label" de la interfaz grafica
            ui->label->clear();
            //Muestra la Matriz de Pixeles en la etiqueta "label" de la interfaz grafica
            ui->label->setPixmap(mapaPixeles);
           }
            }

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    //Crea un cronometro
    QTimer *cronometro = new QTimer(this);
    //Configurar el cronometro
    connect(cronometro, SIGNAL(timeout()), this, SLOT(Temporizador()));
    //Inicia el cronometro
    cronometro->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_6_clicked()
{
    conexionWeb("http://Modular.local", "{\"Servomotor\":\"1\"}");
}

void MainWindow::on_pushButton_7_clicked()
{
    conexionWeb("http://Modular.local", "{\"Servomotor\":\"2\"}");
}

void MainWindow::on_pushButton_9_clicked()
{
    conexionWeb("http://Modular.local", "{\"Servomotor\":\"3\"}");
}

void MainWindow::on_pushButton_8_clicked()
{
    conexionWeb("http://Modular.local", "{\"Servomotor\":\"4\"}");
}

void MainWindow::on_pushButton_10_clicked()
{
    conexionWeb("http://Modular.local", "{\"Servomotor\":\"5\"}");
}
