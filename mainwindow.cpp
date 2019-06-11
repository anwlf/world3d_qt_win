/***********************************************************/
/*   World 3D QT, World3DTread, MainWidow implementation   */
/*   A. Volkov 2010                                        */
/***********************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QTime>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QProcess>

#include "math3d/far_object.h"
#include "solar.h"
#include "tech.h"

//Just for debug
//#define KEY_DEBUG
//Turns on demo for importance of process syncronization
//#define THREAD_SYNC_ON_OFF

World3DThread::World3DThread() : QThread(), World3D()
{
    Build();
    lost_cadres=0;
    step_interval_ms=100;
    //test 12.27.2015
    target = NULL;
    //
}

QString base_name="";

void World3DThread::SwitchBase()
{
    int bn = ++base_num%(base_count+1);
    if (bn==base_count) {
        Base = this;
        base_name = "";
    }
    else {
        Base = bases[bn].Base();
        base_name = bases[bn].Name();
    }
    //SetBase(Base); //added 12.29.2015 not working good
}

sphere star::shape = sphere(32);
sphere planet::shape = sphere(16);
//sphere_points_shape planet::shape = sphere_points_shape();
sphere planetoid::shape = sphere(12);
ring planet_ring::shape = ring(32,0.001,2,1.2);
ellipsoid ellipsoid_saucer::shape=ellipsoid(32,1,0.3);
//ellipsoid ellipsoid_saucer::shape=ellipsoid(64,1,0.3);
//ellipsoid_points_shape ellipsoid_saucer::shape=ellipsoid_points_shape(32,1,1,1);
//cube cube_saucer::shape=cube();
//sphere cube_saucer::shape = sphere(16);
sphere cube_saucer::shape = sphere(32);
//sphere_points_shape cube_saucer::shape=sphere_points_shape(0.1);
//cone cone_saucer::shape=cone(32,0.2);
//cone cone_saucer::shape=cone(32,1.8);
//cone cone_saucer::shape=cone(64,1.8);
cone cone_saucer::shape=cone(64,0.2);
//cone cone_saucer::shape=cone(5,1.8); //for gradient test
//torus space_station::shape_0=torus(8);
//cylinder space_station::shape_1=cylinder(8,1.2);
torus space_station::shape_0=torus(32);
cylinder space_station::shape_1=cylinder(32,1.2);

void World3DThread::Build(basis* b)
{
 Sh_count = 8;
 Ob_count = 7; //Test space_station
 //Shapes  = new facet_shape[Sh_count];
 Shapes  = new shape[Sh_count]; //01.23.2016
 Objects = new object*[Ob_count];
 //
 base_count = 4;
 bases = new base[base_count];
 base_num=base_count;//-1; //1.17.2016
 // Some basic shapes
 Shapes[0]=cube();
 Shapes[1]=cylinder(8,1.2);
 Shapes[2]=cone(16,0.2);
 Shapes[3]=sphere(32,3); //Shapes[3]=sphere(32);
 Shapes[4]=sphere(16);
 //
 Shapes[5]=torus(8);
 Shapes[6]=ellipsoid(32,1,0.3);
 //Shapes[7]=ring(32,0.001,1,0.6);
 Shapes[7]=ring(32,0.001,2,1.2);
 //
 Objects[0] = new Sun(basis(location(  0,  0,50000)));
 //
 Objects[1]=new object(basis(location(-370,  30,5000)),Shapes[5],0xffffff,10); // Spaceship
 Objects[2]=new object(basis(location(-370,  30,5000)),Shapes[1],0xffffff,7);
 //
 //Objects[3]=new object(basis(location(-20,-20, 100)),Shapes[6],0x7f7f7f,10);   // Saucer 1
 Objects[3]=new ellipsoid_saucer(0.1,basis(location(-20,-20, 100)),/*0x7f7f7f*/0xffffff,10,((Sun*)Objects[0])->jupiter());
 //bases[0]=base(Objects[3],vector(0,0,0),vector(0, 0.01,0),"ellipsoid");
 bases[0]=base(Objects[3],vector(0,0,0),vector(0,0,0),"ellipsoid");
 //
 //Objects[4]=new object(basis(location( 0, -20, 100)),Shapes[0],0x7f707f,3);    //Cube
 Objects[4]=new cube_saucer(0.1,basis(location( 0, -20, 100)),0x7f707f,3,((Sun*)Objects[0])->jupiter());
 //bases[1]=base(Objects[4],vector(0,0,0),vector(0.01,0.01,0.01),"cube");
 bases[1]=base(Objects[4],vector(0,0,0),vector(0,0,0),"cube");
 //
 //Objects[5]=new object(basis(location( 20,-20, 100)),Shapes[2],0xffffff,10);   // Saucer 2
 Objects[5]=new cone_saucer(0.1,basis(location( 20,-20, 100)),0xffffff,10,((Sun*)Objects[0])->jupiter());
 //bases[2]=base(Objects[5],vector(0,0,0),vector(0, -0.01,0),"cone");
 bases[2]=base(Objects[5],vector(0,0,0),vector(0,0,0),"cone");
 //01.08.2016 test space_station
 Objects[6]=new space_station(0.1,basis(location(0,20, 100)),0xffffff,2,((Sun*)Objects[0])->jupiter());
 bases[3]=base(Objects[6],vector(0,0,0),vector(0,0,0),"space_station");
 //
 //Far Objects
 far_objects_count = 2000;
 far_objects = new far_object[far_objects_count];
 srand(1000);
 unsigned int colors[5] = {0xffffff,0xffff00,0xff00ff,0x00ffff,0xf0f0f0};
 for (int i=0; i<far_objects_count; i++) {
     far_objects[i]=far_object(vector(rand()%1000-500,rand()%1000-500,rand()%1000-500),colors[rand()%5]);
 }
 //
 //test 12.07.2015
 stars_count = 1000;
 stars = new star*[stars_count];
 for (int i=0; i<stars_count; i++) {
     //vector _ort = ort(location((rand()%300-150),(rand()%300-150),(rand()%300-150)));
     vector _ort = ort(location((rand()-RAND_MAX/2),(rand()-RAND_MAX/2),(rand()-RAND_MAX/2)));
     _ort[1]/=4;
     stars[i] = new star(Planet_absolute_mass(Sun_mass),basis((rand()%30000000+1500000)*_ort),colors[rand()%5],(Sun_size)*(rand()%10 + 1)/2.0,rand()%10+1,0.01*(rand()%10+1));
     //stars[i] = new star(Planet_absolute_mass(Sun_mass),basis((rand()%300000+15000)*100*_ort),colors[rand()%5],(Sun_size)*(rand()%5 + 1)/2.0,rand()%10,0.1);
 }
 //
 World3D::Build(b);
 SetLight(ort(*Objects[0]));
}

bool showLock = false;

image* World3DThread::create_image(video_params* v_p) {
    image* im = new image();
    //Far Objects 1.3.2016
    for (int i=0; i<far_objects_count; i++) {
        (*im)+=far_objects[i].create_image(*Base,v_p);
    }
    //
    for (int i=0; i<stars_count; i++) {
        (*im)+=stars[i]->create_image((*Base/*this*/),Light,v_p); //*Base changed to *this 12.29.2015 - not working good
    }
    //
    //Test Blinking objects 12.15.2015
#ifdef BLINKING
    static unsigned b=0;
    if (b++%10==0) SetLight(vector(-0.0,-0.0,-1.0));
    else SetLight(ort(*Objects[0]));
#endif
    //tmp 02.04.2016
    SetLight(vector(-0.0,-0.0,-1.0));
    //
    (*im) += World3D::create_image(v_p);
    //test 12.30.2015
    if (showLock) {
      object* bt=NULL;
      if (Base==this) bt = target;
      else bt = bases[base_num%base_count].target;
      //
      if (bt!=NULL) {
        location l = (*Base)(*(location*)bt);
        REAL_TYPE d=(l[2]);
        if (d>0) {
          int x =v_p->max_x/2+v_p->scale*(l[0]/d);
          int y =v_p->max_y/2-v_p->scale*v_p->aspect_ratio*(l[1]/d);
          //
          int r = 5;
          (*im)+=new lock_image(0xFFFFFF,x,y,r,NULL,d);
        }
      }
    }
    //
    return im;
}

void base::step_of_life()
{
  //(*((tech_object*)_obj)->Speed())=_speed;
  //(*((tech_object*)_obj)->Rot())=_rotation;
  ((tech_object*)_obj)->step_of_life();
  //_obj%=_rotation;
  //*_obj>>=_speed;
  (*(ph_body*)_obj).step_of_life(_speed,_rotation);
  if (target!=NULL) {
      _rotation = -(vector(0,0,1.0) % (*_obj)(ort((((location)*_obj))-(location)(*target))))/5.0;
     target_spd = abs((location)(*target) - prev_loc);
     prev_loc = (location)(*target);
     if (target==prev_target) {
       _speed = vector(0,0, target_spd);
     }
     //1.10.2016 test
     //(*(*(tech_object*)_obj).Speed())=_speed;
     //(*(*(tech_object*)_obj).Rot())=_rotation;
     prev_target=target;
  }
  //(*(ph_body*)_obj).step_of_life();
  //(*(ph_body*)_obj).step_of_life(_speed,_rotation);
}

void World3DThread::Step_of_life()
{
  //float i=(float)step/M_PI;
 //
 (*((Sun*)Objects[0])).step_of_life(); // commented 1.7.2016 - test gravity body, 12.09.2015 - test solar
 //(*((ph_body*)Objects[3])).step_of_life();//01.05.2016
 //vector r(-0.01,0.03,0.01);
 vector r(-0.01,-0.01,0.001);
 //vector s(0,0,-10);
 vector s(0,0,-20);
 vector v(/*sin(i*0.05)*10.0*/0,0,-sin(/*i*/((float)step/M_PI)*0.05)*100.0);
 (*Objects[1])%=r; // Commented 12.09.2015 - test solar
 (*Objects[1])+=v; // Commented 12.09.2015 - test solar
 (*Objects[1])>>=s; // Commented 12.09.2015 - test solar
 //
 memcpy(&(object::basis&)*Objects[2],&(object::basis&)*Objects[/*1*/1],sizeof(basis));
 //
 for (int i=0; i<base_count; i++) bases[i].step_of_life();
 //
 if (Base==this) World3D::Step_of_life();
 else step++;
 //
 for (int i=0; i<stars_count; i++) {
     stars[i]->step_of_life();
 }
 static REAL_TYPE target_spd = 0;
 static object* prev_target = NULL;
 static vector prev_loc;
 if (target!=NULL) {
    Rot = -(vector(0,0,1.0)/*ort(Speed)*/ % (*this)(ort((((location)*this))-(location)(*target))))/5.0;
    target_spd = abs((location)(*target) - prev_loc);
    prev_loc = (location)(*target);
    if (target==prev_target) {
      Speed = vector(0,0, target_spd);
    }
    prev_target=target;
 }
}

#ifdef THREAD_SYNC_ON_OFF
bool sync_on = true;
#endif

void World3DThread::run () {
    QTime time;
    int rest_to_sleep;
    time.start();
    static long int _step = 1;
STEP:
    time.restart();
    if (
#ifdef THREAD_SYNC_ON_OFF
        (!sync_on)||
#endif
        this->tryLock()) {
        while (step!=_step) this->Step_of_life();
        emit step_finished(time.elapsed());
#ifdef THREAD_SYNC_ON_OFF
        if (sync_on)
#endif
        this->unlock();
    } else {
        lost_cadres++;
    }
    _step++;
    if ((rest_to_sleep=(step_interval_ms-time.elapsed()))>0) msleep(rest_to_sleep);
    goto STEP;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->setBackgroundRole(QPalette::ToolTipBase);
    ui->statusBar->addWidget(ui->label);
    ui->label->setText("");
    ui->label->setAutoFillBackground(true);
    //12.09.2015 - info interface
    ui->groupBox->setVisible(false);
    //12.10.2015 test list widget
    //ui->listWidget->setAttribute(Qt::WA_TranslucentBackground,true);
    //ui->listWidget->window()->setWindowOpacity(0.0);
    //12.09.2015target selector
    ui->widget->setVisible(false);
    ui->listWidget->setVisible(false);
    //
    connect(ui->action_Background,SIGNAL(triggered()),this,SLOT(on_action_Background()));
    connect(ui->action_Step_Interval,SIGNAL(triggered()),this,SLOT(on_action_Step_Interval()));
    connect(ui->action_Exit,SIGNAL(triggered()),this,SLOT(on_action_Exit()));
    connect(ui->action_Keys,SIGNAL(triggered()),this,SLOT(on_action_Keys()));
    connect(ui->action_About,SIGNAL(triggered()),this,SLOT(on_action_About()));
    //List 12.25.2015
    //connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(listCurrentRowChanged(int)));
    //
    world_thread = new World3DThread();
    vs = new qt_video_system(this,/*new QImage(":pic.jpg")*/NULL);
    step_time = image_time = gdi_time =  0;
    im=NULL;
    connect(world_thread,SIGNAL(step_finished(qint64)),this,SLOT(step_event(qint64)));
    world_thread->start(QThread::LowestPriority);
}

MainWindow::~MainWindow()
{
    world_thread->quit();
    delete ui;
    delete vs;
    delete world_thread;
}

void MainWindow::step_event(qint64 st_time)
{
    step_time=st_time;
    if (!im) {
        QTime time;
#ifdef THREAD_SYNC_ON_OFF
        if (sync_on) {
#endif
            // Locks thread to prevent changes during projection
            this->world_thread->lock();
            int max_h = this->height()-ui->statusBar->height();
            //vs->set_video_params(max_h,this->width(),max_h,1);
            //test 12.08.2015
            vs->set_video_params(max_h,this->width(),max_h,1);
            //
            time.start();
            im=world_thread->create_image(vs);
            image_time = time.elapsed();
            time.restart();
            vs->cadre(*im);
            /*tmp test
            vs->setup_cadre();
            world_thread->draw_far_objects(vs);
            im->draw(vs);
            vs->show_cadre();
            */
            //im->clear(); //test 12.31.2015 not works
            delete im;
            im=NULL;
            pixmap_time = time.elapsed();
            // Unlocks thread
            this->world_thread->unlock();
#ifdef THREAD_SYNC_ON_OFF
        } else {
            int max_h = this->height()-ui->statusBar->height();
            vs->set_video_params(max_h,this->width(),max_h,1);
            time.start();
            im=world_thread->create_image(vs);
            image_time = time.elapsed();
            time.restart();
            vs->cadre(*im);
            delete im;
            im=NULL;
            pixmap_time = time.elapsed();
        }
#endif
        update();
        update_status();
    }
}

#ifdef KEY_DEBUG
int k=0;
#endif

void MainWindow::update_status()
{
    QString n;
    QString info("Step time (ms): ");
    info+=n.setNum(step_time);
    info+=", projection time (ms): ";
    info+=n.setNum(image_time);
    info+=", pixmap time (ms): ";
    info+=n.setNum(pixmap_time);
    info+=", GDI time (ms): ";
    info+=n.setNum(gdi_time);
    info+=", lost cadres: ";
    info+=n.setNum(world_thread->lost_cadres);
#ifdef THREAD_SYNC_ON_OFF
    info+=", process synchronization: ";
    if (sync_on) info+="On";
    else info+="Off";
#endif
#ifdef KEY_DEBUG
    info+=", key: ";
    info+=n.setNum(k);
#endif
    if (base_name!="") {
        info+=", base name: ";
        info+=base_name;
    }
   // var os = require("os");
   // info+=", Free Memory: " + QString::number(os.freemem()) ; //12.31.2015
    ui->label->setText(info);
    //12.09.2015
    ui->groupBox->setTitle("Information " + base_name);
    if (world_thread->Base==world_thread->bases[world_thread->base_num%world_thread->base_count].Base()) {
        int bn=world_thread->base_num%world_thread->base_count;
        ui->speed_label->setText("Speed: " + QString::number((*world_thread->bases[bn].Speed())[0]) + "," + QString::number((*world_thread->bases[bn].Speed())[1]) + "," + QString::number((*world_thread->bases[bn].Speed())[2]));
        ui->location_label->setText("Location: " + QString::number((*(location*)(this->world_thread->Base))[0]) + "," + QString::number((*(location*)(this->world_thread->Base))[1]) + "," + QString::number((*(location*)(this->world_thread->Base))[2]));
        ui->rotation_label->setText("Rotation: " + QString::number((*world_thread->bases[bn].Rot())[0]) + "," + QString::number((*world_thread->bases[bn].Rot())[1]) + "," + QString::number((*world_thread->bases[bn].Rot())[2]));
        ui->basis_label->setText("Basis:\t" + QString::number((*(matrix*)this->world_thread->Base)[0][0]) + "," + QString::number((*(matrix*)this->world_thread->Base)[0][1]) + "," + QString::number((*(matrix*)this->world_thread->Base)[0][2]) + "\r\n\t" +
                                              QString::number((*(matrix*)this->world_thread->Base)[1][0]) + "," + QString::number((*(matrix*)this->world_thread->Base)[1][1]) + "," + QString::number((*(matrix*)this->world_thread->Base)[1][2]) + "\r\n\t" +
                                              QString::number((*(matrix*)this->world_thread->Base)[2][0]) + "," + QString::number((*(matrix*)this->world_thread->Base)[2][1]) + "," + QString::number((*(matrix*)this->world_thread->Base)[2][2]));

    } else {
        ui->speed_label->setText("Speed:\t" + QString::number(this->world_thread->Speed[0]) + "," + QString::number(this->world_thread->Speed[1]) + "," + QString::number(this->world_thread->Speed[2]));
        ui->location_label->setText("Location:\t" + QString::number((*(location*)(this->world_thread->Base))[0]) + "," + QString::number((*(location*)(this->world_thread->Base))[1]) + "," + QString::number((*(location*)(this->world_thread->Base))[2]));
        ui->rotation_label->setText("Rotation:\t" + QString::number(this->world_thread->Rot[0]) + "," + QString::number(this->world_thread->Rot[1]) + "," + QString::number(this->world_thread->Rot[2]));
        ui->basis_label->setText("Basis:\t" + QString::number((*(matrix*)this->world_thread)[0][0]) + "," + QString::number((*(matrix*)this->world_thread)[0][1]) + "," + QString::number((*(matrix*)this->world_thread)[0][2]) + "\r\n\t" +
                                              QString::number((*(matrix*)this->world_thread)[1][0]) + "," + QString::number((*(matrix*)this->world_thread)[1][1]) + "," + QString::number((*(matrix*)this->world_thread)[1][2]) + "\r\n\t" +
                                              QString::number((*(matrix*)this->world_thread)[2][0]) + "," + QString::number((*(matrix*)this->world_thread)[2][1]) + "," + QString::number((*(matrix*)this->world_thread)[2][2]));
    }
}

void MainWindow::paintEvent(QPaintEvent * event)
{
    QTime time;
    time.start();
    vs->update_display(event->rect());
    //tmp
    //world_thread->draw_far_objects();
    //
    gdi_time = time.elapsed();
}

void MainWindow::on_action_Background()
{
    QFileDialog	dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.bmp)"));
    if (dialog.exec()) vs->setBackground(new QImage(dialog.selectedFiles()[0]));
}

void MainWindow::on_action_Step_Interval()
{
    QInputDialog dialog(this);
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMaximum(10);
    dialog.setIntMaximum(1000);
    dialog.setIntValue(world_thread->step_interval_ms);
    dialog.setLabelText("Enter step interval 10..1000 (ms)");
    if (dialog.exec()==QDialog::Accepted) {
        world_thread->step_interval_ms = dialog.intValue();
    }
}

void MainWindow::on_action_Exit()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if (QMessageBox::question(this,"Exit","Do you want to stop applicatrion?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes) {
        world_thread->terminate();
        world_thread->quit();
        event->accept();
    } else event->ignore();
}

void MainWindow::on_action_Keys()
{
    help();
}

void MainWindow::on_action_About()
{
    QMessageBox::about(this,"World 3D Application","Developed by A. Volkov, 2010\r\nMath3d Classes were developed in 1991.\r\nTested with Qt in 2010.");
}

void MainWindow::help()
{
    QMessageBox::about(this,"Word 3D Keys","Rotations X\tup/down\r\nRotations Y\tleft/right\r\nRotations Z\tpg_up/pg_down\r\nSpeed Z\t\tins/del\r\nJumps\t\t+/-\r\nSwitch bases\tTab\r\nReset view\t\tESC\r\nStop moving\tothers"
#ifdef THREAD_SYNC_ON_OFF
                       +QString("\r\n\r\nProcess sync.\tBackspace")
#endif
                       );
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    static basis b;
    vector* r;
    vector* s;
    basis* _b;
    object* _t;
    int bn=world_thread->base_num%(world_thread->base_count+1);
    if ((bn<world_thread->base_count)&&(world_thread->Base==world_thread->bases[bn].Base())) {
        s=world_thread->bases[bn].Speed();
        r=world_thread->bases[bn].Rot();
        _b=world_thread->bases[bn].Base();
        _t=world_thread->bases[bn].target;
    } else {
        r=&world_thread->Rot;
        s=&world_thread->Speed;
        _b=world_thread->Base;
        _t=world_thread->target;
    }
#ifdef KEY_DEBUG
    k=event->key();
    switch (k)
#else
    switch (event->key())
#endif
    {
    case Qt::Key_Escape     : {
            //resets to default base
            b=basis();
            //world_thread->base_num=-1;
            world_thread->base_num=world_thread->base_count;
            world_thread->SetBase(&b);
            //*_b=basis();
            //
            world_thread->Rot=vector(0,0,0);
            world_thread->Speed=vector(0,0,0);
            world_thread->target = NULL;
            base_name="";
        } break;
    case Qt::Key_Home       : {
            *r=*s=vector(0,0,0);
            *_b=basis();
        }break;
    case Qt::Key_L          : {
          /*
          QMessageBox* msgBox = new QMessageBox();
          msgBox->setWindowTitle("Key pressed");
          msgBox->setText("Key L");
          msgBox->show();
          */
          disconnect(ui->listWidget,0,0,0);
          //world_thread->target=NULL; //commented 12.30.2015
          //Test 12.18.2015
          ui->listWidget->clear();
          for (int i=0; i<world_thread->Ob_count; i++) {
            if (world_thread->Objects[i]!=world_thread->Base) {
                ui->listWidget->addItem("O " + QString::number(i));
                QVariant var;
                var.setValue(world_thread->Objects[i]);
                ui->listWidget->item(ui->listWidget->count()-1)->setData(1,var);
            }
          }
          connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(listCurrentRowChanged(int)));
          //
          ui->listWidget->move(this->width()-ui->listWidget->width(),ui->listWidget->y());
          ui->listWidget->setVisible(true);
        } break;
    case Qt::Key_PageUp     : *r-=vector(0,0,0.01); break;
    case Qt::Key_PageDown   : *r+=vector(0,0,0.01); break;
    case Qt::Key_Left       : *r-=vector(0,0.001,0); break;
    case Qt::Key_Up         : *r+=vector(0.001,0,0); break;
    case Qt::Key_Right      : *r+=vector(0,0.001,0); break;
    case Qt::Key_Down       : *r-=vector(0.001,0,0); break;
    case Qt::Key_Insert     : *s+=vector(0,0,1.0); break;
    case Qt::Key_Delete     : *s-=vector(0,0,1.0); break;
    case Qt::Key_Plus       : *world_thread->Base>>=vector(0,0,10); break;
    case Qt::Key_Minus      : *world_thread->Base<<=vector(0,0,10); break;
    case Qt::Key_Tab        : world_thread->SwitchBase(); break;
    case Qt::Key_Space      : {
          if (_t!=NULL) {
            vector jump = (((*_t) -  (*((location*)world_thread->Base)))*0.9);
            //*world_thread->Base>>=jump;
            *world_thread->Base+=jump;
          }
       } break;  // jump
    case Qt::Key_F1         : help(); break;
    case Qt::Key_F3         : ui->groupBox->setVisible(showLock=!ui->groupBox->isVisible());break;//12.09.2015
#ifdef THREAD_SYNC_ON_OFF
    case Qt::Key_Backspace  : sync_on=!sync_on; break;
#endif
    default : {
            *r=*s=vector(0,0,0);
        }
    }
}

//test data connection to the list 12.27.2015
Q_DECLARE_METATYPE(object*)
Q_DECLARE_METATYPE(star*)
Q_DECLARE_METATYPE(planet*) //02.17.2017

void MainWindow::mouseReleaseEvent(QMouseEvent* event) //12.09.2015
{
    if (event->button()==Qt::LeftButton) {
        /*
        QMessageBox* msgBox = new QMessageBox();
        msgBox->setWindowTitle("Mouse");
        msgBox->setText("Left Button " + QString::number(event->x()) + "," + QString::number(event->y()));
        msgBox->show();
        */
        //12.10.2015
        ui->widget->setVisible(false);
        ui->listWidget->setVisible(false);
        //ui->widget->pos().setX(event->x())-ui->widget->size().width()/2);
        //ui->widget->pos().setY(event->y());//-ui->widget->size().height()/2);
        ui->widget->move(event->x()-14,event->y()-35);
        //Test 12.27.2015
        //ui->listWidget->setCurrentRow(-1);
        //test 12.27.2015
        disconnect(ui->listWidget,0,0,0);
        //world_thread->target=NULL; //commented 12.30.2015
        //Test 12.18.2015
        ui->listWidget->clear();
        //test 12.20.2015
        int max_h = this->height()-ui->statusBar->height();
        //vs->set_video_params(max_h,this->width(),max_h,1);
        //test 12.08.2015
        vs->set_video_params(max_h,this->width(),max_h,1);
        //
        for (int i=0; i<world_thread->Ob_count; i++) {
          if (world_thread->Objects[i]->visible(*world_thread->Base,vs,ui->widget->x(),ui->widget->y()+ui->statusBar->height(),ui->widget->width(),ui->widget->height())) {
              basis& v_b = *world_thread->Base;
              video_params* v_p = vs;
              location loc = v_b(*(location*)(world_thread->Objects[i]));
              int x = loc[0]*v_p->scale/loc[2] + vs->max_x/2;
              int y = loc[1]*v_p->scale*v_p->aspect_ratio/loc[2]+vs->max_y/2;
              //ui->listWidget->addItem(QString::number(ui->widget->x()) + ":" + QString::number(ui->widget->y()) + ", " + QString::number(x) + ":" + QString::number(y));
              ui->listWidget->addItem("O " + QString::number(x) + ":" + QString::number(y) + " ,p " + QString::number(ui->widget->x()) + ":" + QString::number(ui->widget->y()));
              //test data connection to the list 12.27.2015
              QVariant var;
              var.setValue(world_thread->Objects[i]);
              ui->listWidget->item(ui->listWidget->count()-1)->setData(1,var);
          } else {
              /*
              basis& v_b = *world_thread->Base;
              video_params* v_p = vs;
              location loc = v_b(*(location*)(world_thread->Objects[i]));
              int x = loc[0]*v_p->scale/loc[2] + vs->max_x/2;
              int y = loc[1]*v_p->scale*v_p->aspect_ratio/loc[2]+vs->max_y/2;
              //ui->listWidget->addItem(QString::number(ui->widget->x()) + ":" + QString::number(ui->widget->y()) + ", " + QString::number(x) + ":" + QString::number(y));
              ui->listWidget->addItem("N " + QString::number(x) + ":" + QString::number(y) + " ,p " + QString::number(ui->widget->x()) + ":" + QString::number(ui->widget->y()));
              */
          }
        }
        for (int i=0; i<world_thread->stars_count; i++) {
          if (world_thread->stars[i]->visible(*world_thread->Base,vs, ui->widget->x(),ui->widget->y()+ui->statusBar->height(),ui->widget->width(),ui->widget->height())) {
              ui->listWidget->addItem("Star");
              //world_thread->stars[i]->selected = true;    //12.22.2015 - testing
              //world_thread->Rot = -(vector(0,0,1.0) % ort((((location)*world_thread->Base))-(location)(*(world_thread->stars[i]))))/100.0; //12.24.2015 - tmp  about to work
              //test data connection to the list 12.27.2015
              QVariant var;
              var.setValue(world_thread->stars[i]);
              ui->listWidget->item(ui->listWidget->count()-1)->setData(1,var);
          } //else world_thread->stars[i]->selected = false; //12.22.2015 - testing
          for (int j=0; j<world_thread->stars[i]->planets_count; j++) {
              if (world_thread->stars[i]->planets[j]->visible(*world_thread->Base,vs, ui->widget->x(),ui->widget->y()+ui->statusBar->height(),ui->widget->width(),ui->widget->height())) {
                  ui->listWidget->addItem("Planet");
                  QVariant var;
                  var.setValue(world_thread->stars[i]->planets[j]);
                  ui->listWidget->item(ui->listWidget->count()-1)->setData(1,var);
              }
          }
        }
        //02.17.2016 Solar planets
        Sun* sun = (Sun*)world_thread->Objects[0];
        for (int i=0; i<sun->planets_count; i++) {
            if ((i!=6)&&sun->planets[i]->visible(*world_thread->Base,vs, ui->widget->x(),ui->widget->y()+ui->statusBar->height(),ui->widget->width(),ui->widget->height()))
            {
                //6 - saturn_ring
                ui->listWidget->addItem(((Solar_planet*)sun->planets[i])->name());
                QVariant var;
                var.setValue(sun->planets[i]);
                ui->listWidget->item(ui->listWidget->count()-1)->setData(1,var);
            }
        }
        // 12.25.2015 set rotation by mouse
        //world_thread->Rot = (vector(0,0,1.0) % ort(vector(event->x() - this->width()/2.0, max_h/2.0 - event->y(),0)))/1000.0;
        //
        ui->widget->setVisible(true);
        //ui->listWidget->setFixedSize();
        //test 12.27.2015
        connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(listCurrentRowChanged(int)));
        //
        ui->listWidget->move(this->width()-ui->listWidget->width(),ui->listWidget->y());
        ui->listWidget->setVisible(true);
    } else if (event->button()==Qt::RightButton) {
        ui->widget->setVisible(false);
        ui->listWidget->setVisible(false);
        setFocus();
    }
}

void MainWindow::resizeEvent(QResizeEvent *) //12.24.2015
{
    ui->listWidget->move(this->width()-ui->listWidget->width(),ui->listWidget->y());
}

void  MainWindow::listCurrentRowChanged(int row)
{
  /*
  QMessageBox* msgBox = new QMessageBox();
  msgBox->setWindowTitle("List");
  msgBox->setText("Row " + QString::number(row));
  msgBox->show();
  */
  QVariant var = ui->listWidget->item(row)->data(1);
  /*
  QMessageBox* msgBox = new QMessageBox();
  msgBox->setWindowTitle("List");
  msgBox->setText("Row " + QString::number(row) + ", " + var.typeName());
  msgBox->show();
  */
  int bn=world_thread->base_num%(world_thread->base_count+1);
  if (strcmp(var.typeName(),"star*")==0) {
      star* s = var.value<star*>();
      //if ((world_thread->base_num>-1)&&(world_thread->Base==world_thread->bases[bn].Base())) {
      if ((world_thread->base_num<world_thread->base_count)&&(world_thread->Base==world_thread->bases[bn].Base())) { //1.17.2015
          world_thread->bases[bn].target = s;
      }
      else {
          world_thread->target=s;
      }
  } else if (strcmp(var.typeName(),"planet*")==0) {
      planet* p = var.value<planet*>();
      //if ((world_thread->base_num>-1)&&(world_thread->Base==world_thread->bases[bn].Base())) {
      if ((world_thread->base_num<world_thread->base_count)&&(world_thread->Base==world_thread->bases[bn].Base())) { //1.17.2015
          world_thread->bases[bn].target = p;
      }
      else {
          world_thread->target=p;
      }
  } else {
      object* o = var.value<object*>();
      //if ((world_thread->base_num>-1)&&(world_thread->Base==world_thread->bases[bn].Base())) {
      if ((bn<world_thread->base_count)&&(world_thread->Base==world_thread->bases[bn].Base())) { //1.17.2015
          world_thread->bases[bn].target = o;
          /*
          QMessageBox* msgBox = new QMessageBox();
          msgBox->setWindowTitle("Set Base");
          msgBox->setText("Base " + QString::number(bn));
          msgBox->show();
          */
      }
      else {
          world_thread->target=o;
          /*
          QMessageBox* msgBox = new QMessageBox();
          msgBox->setWindowTitle("Set World Thread");
          msgBox->setText("Base World Thread");
          msgBox->show();
          */
      }
  }
  ui->widget->setVisible(false);
  ui->listWidget->setVisible(false);
  setFocus();
}
