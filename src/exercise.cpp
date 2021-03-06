#include "exercise.h"

#include <QPixmap>
#include <QJsonArray>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

Exercise::Exercise(const QString& name, const QString& img_path, const QString& instruct, const QSet<QString>& tags, QWidget *parent)
    :QLabel(parent), name_(name), img_path_(img_path), instruction_(instruct), tags_(tags)
{
    QPixmap img(img_path_);
    if (img.isNull()){
        throw std::runtime_error("Unable to load image: " + img_path_.toStdString());
    }

    setFixedSize(130, 130);
    setScaledContents(true);
    setPixmap(img);
    setStyleSheet("border: 1px solid black");

    add_button_ = new QPushButton(this);
    add_button_->setGeometry(2, 2, 20, 20);
    add_button_->setStyleSheet("border: 1px solid gray; background: rgba(240,240,240, 150); font-weight: bold");
    add_button_->setFlat(true);
    edit_button_ = new QPushButton("Edit", this);
    edit_button_->setStyleSheet("border: 1px solid gray; background: rgba(240,240,240, 150)");
    edit_button_->setGeometry(23, 2, 40, 20);

}

Exercise::Exercise(const QString& name, const QJsonObject& exercise, QWidget* parent)
    :Exercise(name, exercise["img path"].toString(), exercise["instruction"].toString(), {}, parent)
{
        QJsonArray tags_array = exercise["tags"].toArray();
        for (const auto& tag : tags_array){
            tags_.insert(tag.toString());
        }
}

void Exercise::enterEvent(QEvent *event)
{
    emit Entered();
}

void Exercise::leaveEvent(QEvent *event)
{
    emit Exited();
}

bool Exercise::HasTags(const QSet<QString>& tags)
{
    for (const auto& tag : tags){
        if (!tags_.contains(tag)){
            return false;
        }
    }
    return true;
}

void Exercise::SaveToJson()
{
    QFile file("assets/exercises.json");

    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this, "Unable to open file", "Could not open exercises.json file");
        return;
    }

    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject obj = doc.object();

    QJsonObject ex_obj;
    ex_obj["img path"] = img_path_;
    ex_obj["instruction"] = instruction_;

    QJsonArray tags_arr;
    for (auto& tag : tags_){
        tags_arr.append(tag);
    }
    ex_obj["tags"] = tags_arr;

    obj[name_] = ex_obj;
    doc = QJsonDocument(obj);

    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this, "Unable to open file", "Could not save exercise to exercises.json file");
        return;
    }

    file.write(doc.toJson());

}

void Exercise::DeleteFromJson()
{
    QFile file("assets/exercises.json");
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this, "Unable to open file", "Could not delete exercise from exercises.json file");
        return;
    }

    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject obj = doc.object();
    obj.remove(name_);
    file.open(QIODevice::WriteOnly);
    doc = QJsonDocument(obj);
    file.write(doc.toJson());
    file.close();
}
