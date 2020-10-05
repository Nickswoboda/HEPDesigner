#include "tooltip.h"

#include <QVBoxLayout>
#include <QLabel>

Tooltip::Tooltip(QWidget *parent) : QWidget(parent)
{
	setStyleSheet("background: aliceblue; border: 1px solid black");
    setAttribute(Qt::WA_TransparentForMouseEvents);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    name_label_ = new QLabel("Name");
    name_label_->setWordWrap(true);
    name_label_->setStyleSheet("background: #B0C4DE; font-weight: bold");

    instruction_label_ = new QLabel("Instruction");
    instruction_label_->setWordWrap(true);
    instruction_label_->setStyleSheet("border-bottom: 0px");


    layout->addWidget(name_label_);
	layout->addWidget(instruction_label_);

    setGeometry(0, 0, 165, 160);
}

void Tooltip::SetLabels(const QString& name, const QString& instruction)
{
   name_label_->setText(name);
   instruction_label_->setText(instruction);
}