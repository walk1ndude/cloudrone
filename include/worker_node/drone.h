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