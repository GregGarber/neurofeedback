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

    eeg_data.left.power.stdev = 1.828977738363615;
    eeg_data.right.power.stdev = 1.7559313046810725;
    eeg_data.left.power.mean = 6.123103488904469 ;
    eeg_data.right.power.mean = 6.643216458643411;

    GLWidget *openGL = new GLWidget(&helper, this);
    QLabel *openGLLabel = new QLabel(tr("OpenGL"));
    openGLLabel->setAlignment(Qt::AlignHCenter);

    addConnection("QMYSQL3", "neurofeedback", "localhost", "qt", "yummy23@t", 3306);

    game = new GameController(&helper, &volume_control, &db);
    eeg_data.left.alpha.current = 3.14;
    game->setEEGData(eeg_data);
    qDebug() << "back in window: "<<eeg_data.left.alpha.current;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(openGL, 0, 1);
    layout->addWidget(openGLLabel, 1, 1);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    QTimer *update_timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), openGL, SLOT(animate()));
    connect(update_timer, SIGNAL(timeout()), this, SLOT(getData()));
    connect(&stats_timer, SIGNAL(timeout()), this, SLOT(updateEEGDataStatistics()));

    timer->start(50);
    update_timer->start(100);

    stats_timer.setSingleShot(false);
    stats_timer.setInterval( 1000*60*1 );
    stats_timer.start();


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
    eeg_data.left.power.update(left);
    eeg_data.right.power.update(right);
    if( eeg_data.left.power.current == 0)
        qDebug() << "Electrode Problems? Left Power:" << eeg_data.left.power.current << ", " << left;
    if( eeg_data.right.power.current == 0)
        qDebug() << "Electrode Problems? Right Power:" << eeg_data.right.power.current << ", " << right;
}

void Window::setAlpha(float left, float right){
    left_alpha = (double) left/left_power;
    right_alpha = (double) right/right_power;

    if(eeg_data.left.power.current > 0 ) eeg_data.left.alpha.update(left / eeg_data.left.power.current );
    if(eeg_data.right.power.current > 0 ) eeg_data.right.alpha.update(right / eeg_data.right.power.current );
}

void Window::setBeta(float left, float right){
    current_time = QDateTime::currentDateTime();
    left_beta = (double) left/left_power;
    right_beta = (double) right/right_power;

    if(eeg_data.left.power.current > 0 ) eeg_data.left.beta.update(left / eeg_data.left.power.current );
    if(eeg_data.right.power.current > 0 ) eeg_data.right.beta.update(right / eeg_data.right.power.current );

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

    game->setData();
}

void Window::updateEEGDataStatistics(){
    if(db.isOpen() && session_id > 0 ){
        QString qry;
        qry = QString("SELECT la_stdev, lb_stdev,  lp_stdev, ra_stdev, rb_stdev, rp_stdev \
                      FROM main_stats  \
                      WHERE  session_id = %1 ")
                        .arg(session_id);
                        q.exec(qry);
        if(q.next()){
            eeg_data.left.alpha.stdev = q.value(0).toReal();
            eeg_data.left.beta.stdev = q.value(1).toReal();
            eeg_data.left.power.stdev = q.value(2).toReal();
            eeg_data.right.alpha.stdev = q.value(3).toReal();
            eeg_data.right.beta.stdev = q.value(4).toReal();
            eeg_data.right.power.stdev = q.value(5).toReal();
        }
        q.finish();
    }

    eeg_data.update();
    std::cout << "win "<< " B:A:" << eeg_data.bGa << " RA>:A:" << eeg_data.raGla
              << " LB:RB:" << eeg_data.lbGrb << " RA:RB:" << eeg_data.raGrb << " LB:LA:" << eeg_data.lbGla << std::endl;
    game->computeGameMode();
}
