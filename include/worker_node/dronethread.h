#include <qt/QtCore/QObject>
#include <qt/QtCore/QString>
#include <qt/QtCore/QThread>
#include <qt/QtCore/QProcess>

class DroneThread : public QThread {
  Q_OBJECT
  
public:
  explicit DroneThread(const QString & program, QObject * parent = 0);
  ~DroneThread();
  
  QProcess * getProcess();
  
private:
  QProcess * process;
  QString program;
  
  void run();
  
};