// right_channel right forehead, encourage more alpha
// left_channel left forehead, reduce alpha

#ifndef WINDOW_H
#define WINDOW_H

#include "helper.h"
#include "volumecontrol.h"
#include "gamecontroller.h"

#include <QWidget>
#include <QtSql>
#include <vrpn_Analog.h>
#include <deque>
#include<iostream>

#define DEQUE_SIZE 32
#define LEFT_CHANNEL 1
#define RIGHT_CHANNEL 0

void VRPN_CALLBACK signal_vrpn_analog_callback(void* user_data, vrpn_ANALOGCB analog);
void VRPN_CALLBACK alpha_vrpn_analog_callback(void* user_data, vrpn_ANALOGCB analog);
void VRPN_CALLBACK beta_vrpn_analog_callback(void* user_data, vrpn_ANALOGCB analog);

struct EEGFreq {
    qint64 count = 0;
    double current = 0;
    double mean = 0.3359;
    double stdev = .11475;
    void update(double current_freq){
        if(current_freq > (mean - (2 * stdev)) && current_freq < (mean + (2 * stdev)) ){
            current = current_freq;
            mean += ( current - mean ) / ( count +1 );
            count++;
        }
        else{
            // qDebug() << "cnt:" <<count << "current:" << current << "mean:" << mean << "rejected" << current_freq;
            current = mean;
        }
    }
};

struct EEGChannel{
    EEGFreq alpha;
    EEGFreq beta;
    EEGFreq power;
};

struct EEGData{
    EEGChannel left;
    EEGChannel right;
    double raGrb =0.5, lbGla =0.5, lbGrb =0.5, raGla =0.5, lbGra =0.5, bGa=.5;
    void update(){
        raGrb = right.alpha.mean/(right.alpha.mean + right.beta.mean);
        lbGla = left.alpha.mean/(left.alpha.mean + left.beta.mean);
        lbGrb = left.beta.mean/(left.beta.mean + right.beta.mean);
        raGla = right.alpha.mean/(right.alpha.mean + left.alpha.mean);
        lbGra = left.beta.mean / ( left.beta.mean + right.alpha.mean );
        bGa   = (left.beta.mean + right.beta.mean)/(right.alpha.mean + left.alpha.mean + left.beta.mean + right.beta.mean);
    }
};

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    QSqlError  addConnection(const QString &driver, const QString &dbName, const QString &host,
                             const QString &user, const QString &passwd, int port);
    Helper helper;
    VolumeControl volume_control;
    GameController* game;
    EEGData eeg_data;
    QTimer stats_timer;
    double avg_deque(std::deque<float> chan_deque);
    void setPower(float left, float right);
    void setAlpha(float left, float right);
    void setBeta(float left, float right);
    QSqlError err;
    QSqlDatabase db;
    QSqlQuery q;
    QString qstr;
    QDateTime start_time;
    QDateTime current_time;
    int session_id, la_scf_xref_id, lb_scf_xref_id, ra_scf_xref_id, rb_scf_xref_id;
    double left_alpha=0;
    double right_alpha=0;
    double left_beta=0;
    double right_beta=0;
    double left_mean_alpha=0;
    double left_mean_beta=0;
    double right_mean_alpha=0;
    double right_mean_beta=0;
    double left_power=1.0;
    double right_power=1.0;
    std::deque<float> beta_left_deque;
    std::deque<float> alpha_left_deque;
    std::deque<float> beta_right_deque;
    std::deque<float> alpha_right_deque;
    /* VRPN Analog object */
    vrpn_Analog_Remote* AlphaVRPNAnalog;
    vrpn_Analog_Remote* BetaVRPNAnalog;
    vrpn_Analog_Remote* SignalVRPNAnalog;

private:
public slots:
    void getData();
    void updateEEGDataStatistics();
};

#endif
