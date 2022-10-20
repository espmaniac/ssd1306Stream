#include "mainwindow.h"

#include <QUdpSocket>

#include <QApplication>

#include <QLabel>

//#include<QDebug>


class Server: public QObject {
  public:
  QLabel *label;
  QUdpSocket *socket;
  QByteArray *Buffer;
  QHostAddress sender;
  uint16_t senderport;

  Server(QObject *parent = nullptr, QString ip = "192,168,0,100"): QObject(parent) {
    label = new QLabel();
    Buffer = new QByteArray();
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress(ip), 7755);
    readyRead();
  }

  ~Server() {
    delete label;
    delete Buffer;
    delete socket;
  }

  void readyRead() {
    qInfo() << "waiting for a connection";
    connect(socket, &QUdpSocket::readyRead, this, [&]() mutable {
      while (socket->hasPendingDatagrams()) {
        Buffer->resize(socket->pendingDatagramSize());
        socket->readDatagram(Buffer->data(), Buffer->size(), &sender, &senderport);
        uint8_t arr[1024];
        memcpy(&arr, (uint8_t*)Buffer->data(), 1024);
        QImage create(128, 64, QImage::Format_MonoLSB);

        for (uint8_t x = 0; x < 128; ++x) {
          for (uint8_t y = 0; y < 8; ++y) { // y < (64/8)
            uint8_t bit = *(arr + (y * 128 + x));
            for (uint8_t i = 0; i < 8; ++i, bit >>= 1) {
              create.setPixel(x, (y << 3) + i, bit & 0x01); // (y / 8) + i
            }
          }
        }

        label->setPixmap(QPixmap::fromImage(create));
        label->setFixedSize(128, 64);
        label->show();
      }
    });
  }
};

int32_t main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.setWindowTitle("ssd1306Stream");
  w.setFixedSize(128, 64);

  Server s(&w, "192,168,0,103");

  return a.exec();
}
