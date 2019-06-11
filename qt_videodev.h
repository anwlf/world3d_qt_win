/***********************************************************/
/*   Video device for QT, header                           */
/*   A. Volkov 2010                                        */
/***********************************************************/
#ifndef QT_VIDEODEV_H
#define QT_VIDEODEV_H

//
#define	 _White          0xffffff
#define	 _Black          0x000000
#define	 _Red            0xff0000
#define	 _Dark_red       0x800000
#define	 _Green          0x00ff00
#define	 _Dark_green     0x008000
#define	 _Blue           0x0000ff
#define	 _Dark_blue      0x000080
#define	 _Cyan           0x00ffff
#define	 _Dark_cyan      0x008080
#define	 _Magenta        0xff00ff
#define	 _Dark_magenta   0x800080
#define	 _Yellow         0xffff00
#define	 _Dark_yellow    0x808000
#define	 _Gray           0xa0a0a4
#define	 _Dark_gray      0x808080
#define	 _Light_gray     0xc0c0c0
//

#include "./math3d/videodev.h"
#include <QPainter>
#include <QImage>

class qt_video_system : public video_system, QPainter
{
 public:

  qt_video_system(QPaintDevice* d, QImage* im = NULL) : video_system(),QPainter(d) {
      b_img=im;
      pixmap=NULL;
  }
  virtual ~qt_video_system() { if (pixmap) delete pixmap; } //12.03.2015
  virtual void set_gradient(gradient* g, int color);
  //virtual void draw_poly(int num_points,int *poly_points,int color, REAL_TYPE value);
  virtual void draw_poly(int num_points, PROJECTION_TYPE* poly_points,int color, REAL_TYPE value); //02.28.2016
  //virtual void draw_poly(int num_points,int *poly_points,int color, /*REAL_TYPE value,*/ gradient* g);
  virtual void draw_poly(int num_points, PROJECTION_TYPE* poly_points,int color, /*REAL_TYPE value,*/ gradient* g); //02.28.2016
  //virtual void draw_point(int x,int y,int color);
  virtual void draw_point(PROJECTION_TYPE x, PROJECTION_TYPE y,int color); //02.28.2016
  //virtual void draw_point(int x,int y,int color, REAL_TYPE value); //value - 01.26.2016,01.27.2016
  virtual void draw_point(PROJECTION_TYPE x, PROJECTION_TYPE y,int color, REAL_TYPE value); //02.28.2016
  //virtual void draw_circle(int x,int y, int r, int color, bool fill=true);
 virtual void draw_circle(PROJECTION_TYPE x, PROJECTION_TYPE y, PROJECTION_TYPE r, int color, bool fill=true); //02.28.2016
  void setBackground(QImage* im)
  {
      if (b_img) delete b_img;
      b_img=im;
  }
  void update_display(QRect rect) {
      if (pixmap) {
        this->begin(this->device());
        this->setViewport(0,0,max_x,max_y);
        this->drawImage(rect, pixmap->toImage(), QRect(0,0,max_x,max_y));
        this->end();
      }
  }

 protected:
  QPixmap* pixmap;
  QPainter painter;
  QImage* b_img;
 public: //Change visibility
  virtual void setup_cadre() {
      if (pixmap) {
          delete pixmap;
      }
      pixmap = new QPixmap(max_x,max_y);
      painter.begin(pixmap);
      if (b_img) painter.drawImage(QRectF(0,0,max_x,max_y),*b_img);
      else painter.fillRect(QRectF(0,0,max_x,max_y),Qt::black);
  }
  virtual void show_cadre()  {
      painter.end();
  }
};

#endif // QT_VIDEODEV_H
