//
// Created by NohaHorowitz on 11/9/2019.
//

#include "GameObservers.h"

Observer::Subject::Subject() {
    observers = new vector<Observer *>;
}

Observer::Subject::~Subject() {
    delete observers;

    observers = nullptr;
}

void Observer::Subject::attach(Observer *observer) {
    observers->emplace_back(observer);
}

void Observer::Subject::detach(Observer *observer_to_delete) {
    auto position = observers->begin();
    for (Observer* observer : *observers) {
        if (observer == observer_to_delete) {
            observers->erase(position);
            break;
        }
        position = position++;
    }
}

void Observer::Subject::notify() {
    for (auto observer : *observers) {
        observer->update();
    }
}

Observer::Observer::Observer(Subject *model) {
    model->attach(this);
}

Observer::PlayerObserver::PlayerObserver(PlayerSubject *model, void (*display_callback)(PlayerSubject*)) : Observer(model) {
    this->model = model;
    this->display_callback = display_callback;
}

Observer::PlayerObserver::~PlayerObserver() {

    model->detach(this);
    model = nullptr;
    display_callback = nullptr;
}

void Observer::PlayerObserver::update() {
    display_callback(model);
}



