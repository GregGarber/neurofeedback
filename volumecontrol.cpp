#include "volumecontrol.h"

VolumeControl::VolumeControl(QObject *parent) : QObject(parent)
{

}

void VolumeControl::setupAlsa(){
    const char *card = "default";
    const char *selem_name = "Master";//or Master
    if(snd_mixer_open(&handle, 0) == 0 ) mixer_opened = true;
    if(mixer_opened){
        //std::cout << "Mixer Opened" << std::endl;
        snd_config_update_free_global();//should make valgrind STFU
        snd_mixer_attach(handle, card);
        snd_mixer_selem_register(handle, NULL, NULL);
        snd_mixer_load(handle);
        snd_mixer_selem_id_alloca(&sid);
        snd_mixer_selem_id_set_index(sid, 0);
        snd_mixer_selem_id_set_name(sid, selem_name);
        elem = snd_mixer_find_selem(handle, sid);
        snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
        //std::cout << "Min:" << min << " Max:" << max << std::endl;
        snd_config_update_free_global();//should make valgrind STFU
    }else{
        std::cout << "Failed to open mixer" << std::endl;
    }
}

void VolumeControl::setMasterAlsaVolume(long volume_left, long volume_right){
    snd_mixer_selem_set_playback_volume(elem,SND_MIXER_SCHN_FRONT_LEFT, long(double(volume_left) * double(max) / 100.0f));
    snd_mixer_selem_set_playback_volume(elem,SND_MIXER_SCHN_FRONT_RIGHT, long(double(volume_right) * double(max) / 100.0f));
    snd_config_update_free_global();//should make valgrind STFU
}

long VolumeControl::getMasterAlsaVolume(){
    long outvol;
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &outvol);
    snd_config_update_free_global();//should make valgrind STFU
    return long( (double(outvol)/double(max))*100.0f);
}
