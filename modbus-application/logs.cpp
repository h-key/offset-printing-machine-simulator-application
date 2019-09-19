#include "alarmalert.h"
#include "logs.h"
#include "ui_logs.h"
#include <QMessageBox>
#include <QTime>

QString Logs::convertType(ComponentMessageType type)
{
    switch (type) {
        case Neutral: return "Neutral";
        case Alarming: return "Alarming";
        case Severe: return "Severe";
    }
}

Logs::Logs(Simulator& simulator, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Logs),
    simulator(simulator)
{
    ui->setupUi(this);

    auto timestamp = QTime::currentTime().toString("hh:mm:ss.zzz");
    for (auto &message : simulator.getMachine()->getMessages()) {
        ui->logs->appendPlainText("[" + timestamp + "] -> " + convertType(message->getType()) + " | " + QString::fromStdString(message->getContent()) + '\n');
        if (message->getType() == Severe) {
            createAlarmPopup(QString::fromStdString(message->getContent()), nullptr, nullptr);
        }
    }

    listener = std::make_shared<LogsMessageReceiver>(*this);
    simulator.getMachine()->getExternalMessageReceivers().push_back(listener);
}

void Logs::createAlarmPopup(QString message, QWidget *component, QWidget *parent)
{
    AlarmAlert * aa = new AlarmAlert(message, component, parent);
    aa->show();
}

Logs::LogsMessageReceiver::LogsMessageReceiver(Logs& logs) : logs(logs) { }

void Logs::LogsMessageReceiver::ReceiveMessage(std::shared_ptr<ComponentMessage> message)
{
    auto timestamp = QTime::currentTime().toString("hh:mm:ss.zzz");
    logs.ui->logs->appendPlainText("[" + timestamp + "] -> " + logs.convertType(message->getType()) + " | " + QString::fromStdString(message->getContent()) + '\n');
    if (message->getType() == Severe) {
        QMetaObject::invokeMethod(&logs, "createAlarmPopup", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdString(message->getContent())), Q_ARG(QWidget*, nullptr), Q_ARG(QWidget*, nullptr));
    }
}

Logs::~Logs()
{
    delete ui;
}
