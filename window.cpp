#include "glwidget.h"
#include "widget.h"
#include "window.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>

//! [0]
void VRPN_CALLBACK signal_vrpn_analog_callback(void* user_data, vrpn_ANALOGCB analog)
{
    Window* win=(Window*)user_data;
    win->setPower(analog.channel[LEFT_CHANNEL], analog.channel[RIGHT_CHANNEL]);
}

void VRPN_CALLBACK alpha_vrpn_analog_callback(void* user_data, vrpn_ANALOGCB analog)
{
    Window* win=(Window*)user_data;
    win->setAlpha(analog.channel[LEFT_CHANNEL], analog.channel[RIGHT_CHANNEL]);
}


void VRPN_CALLBACK beta_vrpn_analog_callback(void* user_data, vrpn_ANALOGCB analog){
    Window* win=(Window*)user_data;
    win->setBeta(analog.channel[LEFT_CHANNEL], analog.channel[RIGHT_CHANNEL]);
}

Window::Window()
{
    setWindowTitle(tr("DLPC Alpha Beta Neurofeedback"));
    volume_control.setupAlsa();

    GLWidget *openGL = new GLWidget(&helper, this);
    QLabel *openGLLabel = new QLabel(tr("OpenGL"));
    openGLLabel->setAlignment(Qt::AlignHCenter);

    addConnection("QMYSQL3", "neurofeedback", "localhost", "qt", "yummy23@t", 3306);
    game = new GameController(&helper, &volume_control, &db);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(openGL, 0, 1);
    layout->addWidget(openGLLabel, 1, 1);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    QTimer *update_timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), openGL, SLOT(animate()));
    connect(update_timer, SIGNAL(timeout()), this, SLOT(getData()));

    timer->start(50);
    update_timer->start(100);



    /* Binding of the Alpha VRPN Analog to a callback */
    AlphaVRPNAnalog = new vrpn_Analog_Remote( "alpha_power@localhost" );
    AlphaVRPNAnalog->register_change_handler( this,  alpha_vrpn_analog_callback );

    /* Binding of the Beta VRPN Analog to a callback */
    BetaVRPNAnalog = new vrpn_Analog_Remote( "beta_power@localhost" );
    BetaVRPNAnalog->register_change_handler( this, beta_vrpn_analog_callback );

    /* Binding of the All Freq VRPN Analog to a callback */
    SignalVRPNAnalog = new vrpn_Analog_Remote( "signal_power@localhost" );
    SignalVRPNAnalog->register_change_handler( this, signal_vrpn_analog_callback );

    //if(false && db.isOpen()){
    if(db.isOpen()){
        qstr=QString("INSERT INTO nf_session (type) VALUES ( 'Alpha Beta DLFP Depression')");
        q.prepare(qstr);
        if( q.exec()){
            start_time = QDateTime::currentDateTime();
            session_id = q.lastInsertId().toInt();
            qDebug() << "session_id:"<<session_id;
            game->setSessionID(session_id);
            q.finish();
        }
    }
   // session_id=80;
     //       game->setSessionID(session_id);
}
QSqlError Window::addConnection(const QString &driver, const QString &dbName, const QString &host,
                            const QString &user, const QString &passwd, int port)
{
    static int cCount = 0;
     db = QSqlDatabase::addDatabase(driver, QString("Browser%1").arg(++cCount));
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if (!db.open(user, passwd)) {
        err = db.lastError();
        qDebug() << err.text() ;
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(QString("Browser%1").arg(cCount));
    }else{
        q = QSqlQuery("", db);
    }
    return err;

}
void Window::getData(){
    SignalVRPNAnalog->mainloop();
    AlphaVRPNAnalog->mainloop();
    BetaVRPNAnalog->mainloop();
}

double Window::avg_deque(std::deque<float> chan_deque){
    double sum = 0.0;
    for(unsigned int i = 0; i< chan_deque.size(); i++){
        sum += chan_deque[i];
    }
    return (sum/chan_deque.size());
}

void Window::setPower(float left, float right){
    left_power = left;
    right_power = right;
}

void Window::setAlpha(float left, float right){
    double right_channel, left_channel;
    left_channel = (double) left/left_power;
    right_channel = (double) right/right_power;

    if(alpha_left_deque.size() == DEQUE_SIZE){
        if( left_channel < 2.0 * avg_deque(alpha_left_deque)){
            alpha_left_deque.push_back(left_channel);
                left_alpha = left_channel;
        }
        else{
                left_alpha = avg_deque(alpha_left_deque);
        }
    }else{
        alpha_left_deque.push_back(left_channel);
    }
    if(alpha_left_deque.size() > DEQUE_SIZE){
        alpha_left_deque.pop_front();
    }
    left_mean_alpha = avg_deque(alpha_left_deque);

    if(alpha_right_deque.size() == DEQUE_SIZE){
        if( right_channel < 2.0 * avg_deque(alpha_right_deque)){
            alpha_right_deque.push_back(right_channel);
        right_alpha = right_channel;
        }
    else{
        right_alpha = avg_deque(alpha_right_deque);

    }
    }else{
        alpha_right_deque.push_back(right_channel);
    }
    if(alpha_right_deque.size() > DEQUE_SIZE){
        alpha_right_deque.pop_front();
    }
    right_mean_alpha = avg_deque(alpha_right_deque);
}

void Window::setBeta(float left, float right){
    double right_channel, left_channel;

    current_time = QDateTime::currentDateTime();
    left_channel = (double) left/left_power;
    right_channel = (double) right/right_power;

    if(beta_left_deque.size() == DEQUE_SIZE){
        if( left_channel < 1.5 * avg_deque(beta_left_deque)){
            beta_left_deque.push_back(left_channel);
            left_beta = left_channel;
        }else{
            left_beta = avg_deque(beta_left_deque);
        }
    }else{
        beta_left_deque.push_back(left_channel);
    }
    if(beta_left_deque.size() > DEQUE_SIZE){
        beta_left_deque.pop_front();
    }
    left_mean_beta = avg_deque(beta_left_deque);

    if(beta_right_deque.size() == DEQUE_SIZE){
        if( right_channel < 1.5 * avg_deque(beta_right_deque)){
            beta_right_deque.push_back(right_channel);
            right_beta = right_channel;
        }else{
            right_beta = avg_deque(beta_right_deque);
        }
    }else{
        beta_right_deque.push_back(right_channel);
    }
    if(beta_right_deque.size() > DEQUE_SIZE){
        beta_right_deque.pop_front();
    }
    right_mean_beta = avg_deque(beta_right_deque);

    //if(false && db.isOpen()){
    if(db.isOpen()){
        qstr=QString("INSERT INTO alpha_beta ")
                     +QString("(session_id, left_alpha, right_alpha, left_beta, right_beta, left_power, right_power, msecs) ")
                     +QString(" VALUES( %1, %2, %3, %4, %5, %6, %7, %8)")
                .arg( session_id)
                .arg(left_alpha)
                .arg(right_alpha)
                .arg(left_beta)
                .arg(right_beta)
                .arg(left_power)
                .arg(right_power)
                .arg(start_time.msecsTo(current_time));
        q.exec(qstr);
        q.finish();
    }
        game->setData(left_alpha,  right_alpha, left_beta, right_beta);
}



//! [0]
