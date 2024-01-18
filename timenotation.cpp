#include<systemc>
#include<iostream>
using namespace sc_core;
using namespace std;

int sc_main(int,char*[]){


//sc_report_handler: is a class that handles reporting of different events or conditions during simulation
//set_actions: method to specify the action to be taken when a specific report is triggered.
//"/IEEE_Std_1666/deprecated": This is the identifier for the type of report you want to set the action for
//SC_DO_NOTHING:is a predefined action that essentially means to take no action
sc_core::sc_report_handler::set_actions("/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING);

//it specifies the smallest time unit that can be used in the simulation envro.
//it means that the simulation can represent and process time increments as small as one femtosecond
sc_set_time_resolution(1,SC_FS);
//
sc_set_default_time_unit(1,SC_SEC); // change time unit to second

cout<<" 1 sec ="<<sc_time(1,SC_SEC).to_default_time_units() << "sec" << endl;

cout<<" 1 MS  ="<<sc_time(1,SC_MS).to_default_time_units() << "sec" << endl;

cout<<" 1 US  ="<<sc_time(1,SC_US).to_default_time_units() << "sec" << endl;
cout<<" 1 NS  ="<<sc_time(1,SC_NS).to_default_time_units() << "sec" << endl;
cout<<" 1 PS ="<<sc_time(1,SC_PS).to_default_time_units() << "sec" << endl;
cout<<" 1 US ="<<sc_time(1,SC_US).to_default_time_units() << "sec" << endl;



// run simulation  600 sec

sc_start(600,SC_SEC);

//get time in second

//current simulation time (sc_time_stamp()) and converting it to seconds
double t = sc_time_stamp().to_seconds();


//prints the time in hours, minutes, and seconds


cout<<int(t)/3600<<"hours"<<" "<<(int(t)%3600)/60<<"minutes, "<<" "<<(int(t)%60)<<"seconds"<<endl;


return 0;

}


