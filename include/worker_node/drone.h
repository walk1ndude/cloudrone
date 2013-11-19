#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QString>
#include <QtCore/QProcess>
#include <QtCore/QDebug>

class Drone : public QObject {
  Q_OBJECT
  
public:
  explicit Drone(QObject * parent = 0);
  ~Drone();
  
  int getID();
  int getPlayPID();
  QProcess * getProcess();
  QVector<std::string> getFlightTask();
  
  void setID(const int & id);
  void setPlayPID(const int & id);
  void setProgram(const QString & program);
  void setFlightTask(const QVector<std::string> & flightTask);
  
private:
  QProcess * process;
  QString program;
  QVector<std::string>flightTask;
  int id;
  int ccommand;
  int playPID; // pid of rosbag, need for play / resume

signals:
  void signalTaskFinished(Drone * drone);
  void signalPublishTum(const int & id);
  
public slots:
  void startTask();
  void publishTum();
};