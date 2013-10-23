#include <qt/QtCore/QObject>
#include <qt/QtCore/QThread>
#include <qt/QtCore/QString>
#include <qt/QtCore/QProcess>
#include <qt/QtCore/QDebug>

class Drone : public QObject {
  Q_OBJECT
  
public:
  explicit Drone(QObject * parent = 0);
  ~Drone();
  
  int getID();
  void setID(const int & id);
  QProcess * getProcess();
  void setProgram(const QString & program);
  
private:
  QProcess * process;
  QString program;
  int id;

signals:
  void signalTaskFinished(Drone * drone);
  
public slots:
  void startTask();
};