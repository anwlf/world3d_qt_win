/***********************************************************/
/*   World 3D QT, World3DTread, MainWidow header           */
/*   A. Volkov 2011                                        */
/***********************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QtGui/QMainWindow>
#include <QtWidgets/QMainWindow>
#include <QThread>
#include <QMutex>
#include "./math3d/World3D.h"
#include "qt_videodev.h"

#include "./math3d/far_object.h"

#include "physics.h"
#include "astronomy.h"
//#include "tech.h"

namespace Ui
{
    class MainWindow;
}

// 1.7.2016
//tech_object(REAL_TYPE m, basis o_b, facet_shape& sh, int color, int size=1, ph_body* gb=NULL) -- not now
class base //: public tech_object
{
protected:
    object *_obj;
    vector _speed;
    vector _rotation;
    QString _name;
public:
    //test 12.29.2015
    object* target;
    REAL_TYPE target_spd;
    object* prev_target;
    vector prev_loc;
    //
    basis* Base() { return _obj; }
    vector* Speed() { return &_speed; }
    vector* Rot() { return &_rotation; }
    QString& Name() { return _name; }
    base(object* obj=NULL, vector spd = vector(), vector rot=vector(), QString n="") { _obj=obj; _speed=spd; _rotation=rot; _name=n; target_spd = 0; target = prev_target = NULL;}
    void step_of_life();// { *_obj%=_rotation; *_obj>>=_speed; } //12.29.2015 moved to the cpp
};

class World3DThread: public QThread, public QMutex, public World3D
{
    Q_OBJECT

    //qt_video_system* video_system;

public:
    qt_video_system* video_system; //12.18.2015
    //
    using World3D::Objects;  //12.18.2015
    using World3D::Ob_count; //12.18.2015
    //test 12.07.2015
    int stars_count;
    star** stars;
    //test 12.27.2015
    object* target;
    //
    int far_objects_count;
    far_object* far_objects;
    World3DThread();
    ~World3DThread() {this->unlock(); quit(); delete [] bases; delete[] stars;}
    int step_interval_ms;
    int lost_cadres;
    base* bases;
    int base_count, base_num;
    void SwitchBase();
    void draw_far_objects(qt_video_system* v_s);
    virtual image* create_image(video_params* v_p); //12.07.2015
signals:
    // "world_thread" has finished calculations  and emits signal
    void step_finished(qint64 step_time);
protected:
    // "World3D" virtual functions
    virtual void Build(basis* b=NULL);
    virtual void Step_of_life();
    //
    virtual void run ();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    qt_video_system* vs;
    World3DThread* world_thread;
    image* im;
    Ui::MainWindow *ui;
    qint64 step_time,image_time,pixmap_time,gdi_time;

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void paintEvent(QPaintEvent * event);
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void closeEvent(QCloseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent* event);//12.09.2015
    virtual void resizeEvent(QResizeEvent *); //12.24.2015
    // Displays simple help dialog
    void help();
public slots:
    // "world_thread" has finished calculations and emitted signal to this slot
    void step_event(qint64 step_time);
    // Shows info about times
    void update_status();
    // Menu actions
    void on_action_Background();
    void on_action_Step_Interval();
    void on_action_Exit();
    void on_action_Keys();
    void on_action_About();
    //listWidget
    void listCurrentRowChanged(int row);
};

#endif // MAINWINDOW_H
