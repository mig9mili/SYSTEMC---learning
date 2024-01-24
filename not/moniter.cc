#include"moniter.h"
#include<iostream>

using namespace std;

void moniter::monit(void){
    cout<<"at "<<sc_time_stamp()<<"input is "<<m_din<<" output is | "<<m_s_out<<" | "<<endl;
}