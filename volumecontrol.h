#ifndef VOLUMECONTROL_H
#define VOLUMECONTROL_H

#include <QObject>
#include<iostream>
#include <alsa/asoundlib.h>

class VolumeControl : public QObject
{
    Q_OBJECT
public:
    explicit VolumeControl(QObject *parent = 0);
    void setupAlsa();
    long getMasterAlsaVolume();

    snd_mixer_t *handle = 0;
    snd_mixer_selem_id_t *sid = 0;
    snd_mixer_elem_t* elem = 0;
    long min, max;
    bool mixer_opened = false;

signals:

public slots:
    void setMasterAlsaVolume(long volume_left, long volume_right);
};

#endif // VOLUMECONTROL_H
