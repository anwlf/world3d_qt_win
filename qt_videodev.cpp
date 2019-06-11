/***********************************************************/
/*   Video device for QT, implementation                   */
/*   A. Volkov 2010                                        */
/***********************************************************/

#include "qt_videodev.h"
#include <stdio.h> //for testing printf

void qt_video_system::set_gradient(gradient* g, int color)
{
    QColor c(color);
    /*tmp solution 02.04.2016
    qreal red=c.redF()*value,
          green=c.greenF()*value,
          blue=c.blueF()*value;
    QColor cm=QColor::fromRgbF(red,green,blue,1); //commented for test 02.07.2016
    */
    if (g==NULL) {
        painter.setBrush(QBrush(c));
        return;
    }
    switch (g->type()) {
        case linear: {
            QLinearGradient gr(((linear_gradient*)g)->start_x,((linear_gradient*)g)->start_y,((linear_gradient*)g)->stop_x,((linear_gradient*)g)->stop_y);
            gr.setStart(((linear_gradient*)g)->start_x,((linear_gradient*)g)->start_y);
            gr.setFinalStop(((linear_gradient*)g)->stop_x,((linear_gradient*)g)->stop_y);
            qreal value = (*(linear_gradient*)g)[0];
            if (value<0.0) value=0.0;  //test 12.06.2015
            qreal red=c.redF()*value,
                  green=c.greenF()*value,
                  blue=c.blueF()*value;
            gr.setColorAt(0,QColor::fromRgbF(red,green,blue,g->transp));
            value = (*(linear_gradient*)g)[1];
                        if (value<0.0) value=0.0;  //test 12.06.2015
                        red=c.redF()*value;
                        green=c.greenF()*value;
                        blue=c.blueF()*value;
            gr.setColorAt(1,QColor::fromRgbF(red,green,blue,g->transp));
            painter.setBrush(gr);
        } break;
        case conical: { 
            qreal angle=((conical_gradient*)g)->angle*180.0/M_PI;
            qreal sangle=((conical_gradient*)g)->stop_angle*180.0/M_PI;
            qreal delta=(sangle-angle)/360.0;
            //printf("angle = %f, sangle = %f, delta=%f\r\n",angle,sangle,delta);
            QConicalGradient gr(((conical_gradient*)g)->center_x,((conical_gradient*)g)->center_y,angle);
            //
            qreal value = (*(conical_gradient*)g)[0]; //1
            if (value<0.0) value=0.0;  //test 12.06.2015
            qreal red=c.redF()*value,
            green=c.greenF()*value,
            blue=c.blueF()*value;
            QColor c1 = QColor::fromRgbF(red,green,blue,g->transp);
            value = (*(conical_gradient*)g)[1]; //0
            if (value<0.0) value=0.0;  //test 12.06.2015
            red=c.redF()*value;
            green=c.greenF()*value;
            blue=c.blueF()*value;
            QColor c2 = QColor::fromRgbF(red,green,blue,g->transp);
            //
            if (delta<0)  gr.setColorAt(1.0+delta,c2);
            else gr.setColorAt(0,c1);
            //test middle collor 02.06.2015
            //gr.setColorAt(delta/2.0,cm);      //commented for test 02.07.2016
            //gr.setColorAt(delta/2.0,Qt::gray);
            if (delta<0) gr.setColorAt(1.0,c1);
            else gr.setColorAt(delta,c2);
            painter.setBrush(gr);
        } break;
        case radial: {  //tmp testing 02.09.2016
            //QRadialGradient gr(((radial_gradient*)g)->center_x,((radial_gradient*)g)->center_y,((radial_gradient*)g)->center_r,((radial_gradient*)g)->focal_x,((radial_gradient*)g)->focal_y, ((radial_gradient*)g)->focal_r);
            QRadialGradient gr(((radial_gradient*)g)->center_x,((radial_gradient*)g)->center_y,((radial_gradient*)g)->center_r);
            qreal v2p = ((radial_gradient*)g)->value_2_point;
            //gr.setStart(((linear_gradient*)g)->start_x,((linear_gradient*)g)->start_y);
            //gr.setFinalStop(((linear_gradient*)g)->stop_x,((linear_gradient*)g)->stop_y);
            qreal value = (*g)[0];//(*(radial_gradient*)g)[0];
            if (value<0.0) value=0.0;  //test 12.06.2015
            qreal red=c.redF()*value,
              green=c.greenF()*value,
              blue=c.blueF()*value;
            gr.setColorAt(v2p,QColor::fromRgbF(red,green,blue,g->transp));
            //gr.setColorAt(v2p,Qt::white);
            value = (*g)[1];//(*(radial_gradient*)g)[1];
                    if (value<0.0) value=0.0;
                    red=c.redF()*value;
                    green=c.greenF()*value;
                    blue=c.blueF()*value;
            gr.setColorAt(1,QColor::fromRgbF(red,green,blue,g->transp));
            //gr.setColorAt(1,Qt::black);
            painter.setBrush(gr);
        } break;
    default: {
            painter.setBrush(QBrush(c));
        } break;
    }
}

/***********************************************************/
/* Drav polygon method implementation                      */
/***********************************************************/
//void qt_video_system::draw_poly(int num_points,int *poly_points,int color,
void qt_video_system::draw_poly(int num_points, PROJECTION_TYPE* poly_points,int color, //02.28.2016
                                 REAL_TYPE value)
{
    // Allocates temporary array for points
    //QPoint* points = new QPoint[num_points]; //commented 12.31.2015
    QPoint points[num_points]; //12.31.2015
    //
    int p=0; // 12.03.2015 - optimize drawing speed
    for (int i=0; i<num_points; i++) {
        points[p].setX(poly_points[i*2]);
        points[p].setY(poly_points[(i*2)+1]);
        //12.03.2015 - optimize drawing speed
        for (int j=0; j<p; j++) {
            if ((points[p].x() == points[j].x()) && (points[p].y() == points[j].y())) {
                p--;
                break;
            }
        }
        p++;
    }
    QColor c(color);
    //Calculates color RGB components intensivity
    if (value<0.0) value=0.0;  //test 12.06.2015
    qreal red=c.redF()*value,
          green=c.greenF()*value,
          blue=c.blueF()*value;
    // Creates color drom RGB
    //test 12.06.2015
    /*
    if (red>1.0) red=1.0;
    if (green>1.0) green=1.0;
    if (blue>1.0) blue=1.0;
    if (red<0.0) red=0.0;
    if (green<0.0) green=0.0;
    if (blue<0.0) blue=0.0;
    */
    c=QColor::fromRgbF(red,green,blue,1);
    // Sets pen and brush
    painter.setPen(c);
    // 12.02.2015 - drawPoint for far_objects
    // 12.03.2015 - optimize drawing speed - p instead num_points
    if (/*num_points*/p > 1) {
        painter.setBrush (QBrush(c));
        // Draws polygon
        painter.drawPolygon(points, /*num_points*/p);
    } else {
        painter.drawPoint(points[0]);
    }
    // Releases memory
    //delete points; //commented 12.31.2015
}

//void qt_video_system::draw_poly(int num_points,int *poly_points,int color, /*REAL_TYPE value,*/ gradient* g)
void qt_video_system::draw_poly(int num_points, PROJECTION_TYPE* poly_points,int color, gradient* g) //02.28.2016
{
    // Allocates temporary array for points
    //QPoint* points = new QPoint[num_points]; //commented 12.31.2015
    QPoint points[num_points]; //12.31.2015
    //
    int p=0; // 12.03.2015 - optimize drawing speed
    for (int i=0; i<num_points; i++) {
        points[p].setX(poly_points[i*2]);
        points[p].setY(poly_points[(i*2)+1]);
        //12.03.2015 - optimize drawing speed
        for (int j=0; j<p; j++) {
            if ((points[p].x() == points[j].x()) && (points[p].y() == points[j].y())) {
                p--;
                break;
            }
        }
        p++;
    }
    QColor c(color);
    //Calculates color RGB components intensivity
    /*
    if (value<0.0) value=0.0;  //test 12.06.2015
    qreal red=c.redF()*value,
          green=c.greenF()*value,
          blue=c.blueF()*value;
    // Creates color drom RGB
    //test 12.06.2015
    / *
    if (red>1.0) red=1.0;
    if (green>1.0) green=1.0;
    if (blue>1.0) blue=1.0;
    if (red<0.0) red=0.0;
    if (green<0.0) green=0.0;
    if (blue<0.0) blue=0.0;
    */
    //c=QColor::fromRgbF(red,green,blue,1);
    // Sets pen and brush
    //painter.setPen(c);
    painter.setPen(Qt::transparent);
    // 12.02.2015 - drawPoint for far_objects
    // 12.03.2015 - optimize drawing speed - p instead num_points
    if (/*num_points*/p > 1) {
        if (g==NULL) painter.setBrush (QBrush(c));
        else {
            if (g->type()==custom) set_gradient(((custom_gradient*)g)->grad[0],color);
            else set_gradient(g,color);
        }
        // Draws polygon
        painter.drawPolygon(points, /*num_points*/p);
        /*test 02.25.2016
        if ((g->type()==custom) && (((custom_gradient*)g)->grad[1]!=NULL) && (((custom_gradient*)g)->grad[1]->type()==radial)) {
            painter.drawPolygon(points, p);
        }*/
        /*
        //test 02.24.2016 - transparent gradient
        qreal x1=(points[0].x()+points[3].x())/2.0,
              x2=(points[1].x()+points[2].x())/2.0,
              y1=(points[0].y()+points[3].y())/2.0,
              y2=(points[1].y()+points[2].y())/2.0;
        QLinearGradient gr(x1,y1,x2,y2);
        qreal red = c.redF(), green = c.greenF(), blue = c.blueF();
        c = QColor::fromRgbF(red,green,blue,1.0);
        gr.setColorAt(1,c);
        c = QColor::fromRgbF(red*0.0,green*0.0,blue*0.0,1.0);
        gr.setColorAt(0,c);
        */
        if ((g->type()==custom) && (((custom_gradient*)g)->grad[1]!=NULL)) {
            set_gradient(((custom_gradient*)g)->grad[1],color);
            painter.drawPolygon(points, p);
        }
        //if (g!=NULL) delete g; //02.09.2016 unsafe
    } else {
        painter.drawPoint(points[0]);
    }
    // Releases memory
    //delete points; //commented 12.31.2015
    //if (g!=NULL) delete g; //test memory leak 02.11.2016
}


//void qt_video_system::draw_point(int x,int y,int color)
void qt_video_system::draw_point(PROJECTION_TYPE x, PROJECTION_TYPE y, int color)  //02.28.2016
{
    QColor c(color);
    painter.setPen(c);
    painter.drawPoint(x,y);
}

//void qt_video_system::draw_point(int x,int y,int color, REAL_TYPE value) //12.08.2015, 01.26.2016
void qt_video_system::draw_point(PROJECTION_TYPE x, PROJECTION_TYPE y,int color, REAL_TYPE value) //02.28.2016
{
    QColor c(color);
    if (value<0.0) value=0.0;  //01.26.2016
    if (value<1.0) {
      qreal red=c.redF()*value,
      green=c.greenF()*value,
      blue=c.blueF()*value;
      c=QColor::fromRgbF(red,green,blue,1);
    }
    painter.setPen(c);
    painter.drawPoint(x,y);
}

//void qt_video_system::draw_circle(int x,int y,int r,int color, bool fill) //12.08.2015
void qt_video_system::draw_circle(PROJECTION_TYPE x, PROJECTION_TYPE y, PROJECTION_TYPE r,int color, bool fill) //02.28.2016
{
    QColor c(color);
    painter.setPen(c);
    if (fill) {
      painter.setBrush(QBrush(c));
    } else {
      painter.setBrush(QBrush(Qt::transparent));
    }
    painter.drawEllipse(x-r,y-r,r*2,r*2);
}

