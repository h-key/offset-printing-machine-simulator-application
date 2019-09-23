#include "countlistener.h"
#include "utility.h"
#include <math.h>

CountListener::CountListener(TempoComponent& tempoComponent, QLabel * label, QLabel * optionalLabel)
    : tempoComponent(tempoComponent), label(label), optionalLabel(optionalLabel) {
    QObject::connect(this, SIGNAL(receiveLabels(QString, QString)), this, SLOT(setLabels(QString, QString)), Qt::ConnectionType::QueuedConnection);
}

void CountListener::setLabels(QString one, QString two)
{
    if (optionalLabel == nullptr) {
        QStringList list = label->text().split('/');
        label->setText(one + '/' + two);
    } else {
        label->setText(Utility::replaceNumbers(label->text(), one));
        optionalLabel->setText(Utility::replaceNumbers(optionalLabel->text(), two));
    }
};

void CountListener::ReceiveMessage(std::shared_ptr<CountMessage> message) {
    emit receiveLabels(QString::number(message->getCount()), QString::number(std::round(message->getPercentage() * 100)) + '%');
}
