#include <QtCore/QCoreApplication>
#include <worker_node/worker.h>

#define NODE_RATE 10

int main(int argc, char ** argv) {
  
  ros::init(argc, argv, "worker_node");
  
  QCoreApplication app(argc,argv);
  
  Worker worker;
  
  ros::Rate rosRate(NODE_RATE);
  
  while(ros::ok()) {
    ros::spinOnce();
    rosRate.sleep();
    
  }
  
  return app.exec();
  
}
