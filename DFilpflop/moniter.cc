#include "moniter.h"
#include<iostream>

using namespace std;

void moniter :: monita(void){
    cout<<"at "<<sc_time_stamp()<<"clear is "<<m_clr<<"input is : "<<m_din<<" | "<<"output is : | "<<m_dout << " | "<<endl;
}