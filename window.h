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
//! [0]
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
    /*
    void setupAlsa();
    void setMasterAlsaVolume(long volume_left, long volume_right);
    long getMasterAlsaVolume();
    snd_mixer_t *handle = 0;
    snd_mixer_selem_id_t *sid = 0;
    snd_mixer_elem_t* elem = 0;
    long min, max;
    bool mixer_opened = false;
    */
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
};
//! [0]

#endif
