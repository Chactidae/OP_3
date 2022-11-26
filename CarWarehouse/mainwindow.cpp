#include "examplevalidator.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "namevalidator.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);                       // Задать id для типа транспорта
    ui->tGroup->setId(ui->bus, 0);
    ui->tGroup->setId(ui->passenger, 1);
    ui->tGroup->setId(ui->cargo, 2);
    ui->tGroup->setId(ui->trailer, 3);


    QValidator *ev = new NameValidator(this);        // Подключение валидатора к названию (Базовое задание)
    ui->name->setValidator(ev);


    // обработчики события "клик"
    connect(ui->table, SIGNAL(cellClicked(int,int)), this, SLOT(setCurrentWarehouse(int, int)));   // показать
    connect(ui->Create, SIGNAL(clicked()), this, SLOT(createWarehouse()));    // сохранить
    // -------
    connect(ui->trailer, SIGNAL(clicked()), this, SLOT(showBasicAttributes())); // Если выбрали тип "прицеп"
    connect(ui->bus, SIGNAL(clicked()), this, SLOT(showBasicAttributes()));     // Если выбрали другой тип
    connect(ui->cargo, SIGNAL(clicked()), this, SLOT(showBasicAttributes()));
    connect(ui->passenger, SIGNAL(clicked()), this, SLOT(showBasicAttributes()));
    // -------
    connect(ui->Save, SIGNAL(clicked()), this, SLOT(saveWarehouse()));
    connect(ui->edit, SIGNAL(clicked()), this, SLOT(currentEdit()));
    connect(ui->Delete, SIGNAL(clicked()), this, SLOT(delWarehouse()));

    ui->engineCapacity->setMaximum(30);              // Прописываем ограничения
    ui->engineCapacity->setMinimum(0.5);
    ui->capacity->setMaximum(300);
    ui->capacity->setMinimum(1);
    ui->loadCapacity->setMaximum(5000);
    ui->loadCapacity->setMinimum(100);
    ui->maxSpeed->setMaximum(320);
    ui->maxSpeed->setMinimum(60);
    ui->accelerationTo100->setMaximum(120);
    ui->accelerationTo100->setMinimum(5);
    ui->count->setMaximum(1000);
    ui->count->setMinimum(0);
    ui->price->setMaximum(3000);
    ui->price->setMinimum(20);                      //
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showBasicAttributes(){              // Закрывать основные характеристики для прицепа
    if (ui->trailer->isChecked()){
        ui->engineCapacity->setEnabled(false);
        ui->maxSpeed->setEnabled(false);
        ui->capacity->setEnabled(false);
        ui->accelerationTo100->setEnabled(false);
        ui->climateControl->setEnabled(false);
    }
    else{
        ui->engineCapacity->setEnabled(true);      // Иначе открываем поля для записи
        ui->maxSpeed->setEnabled(true);
        ui->capacity->setEnabled(true);
        ui->accelerationTo100->setEnabled(true);
        ui->climateControl->setEnabled(true);
    }
}

void MainWindow::fillTable(){
    qDebug() << "count warehouse";
    qDebug() << countWarehouse;
    ui->table->setRowCount(0);
    for (int i = 0; i < countWarehouse; i++){
        for (int j = 0; j < countWarehouse - i - 1; j++){
            if (*warehouseArr[j] > *warehouseArr[j + 1]){
                Warehouse *swapper = warehouseArr[j + 1];
                warehouseArr[j + 1] = warehouseArr[j];
                warehouseArr[j] = swapper;
            }
        }
    }
    for (int y = 0; y < countWarehouse; y++){
        qDebug() << "All elements";
        qDebug() << warehouseArr[y]->getCapacity();
    }
    for (int j = 0; j < countWarehouse; j++){
        ui->table->insertRow(j);
        ui->table->setItem(j, 0, new QTableWidgetItem(warehouseArr[j]->getName()));
        ui->table->setItem(j, 1, new QTableWidgetItem(QString::number(warehouseArr[j]->getCount())));
        ui->table->setItem(j, 2, new QTableWidgetItem(QString::number(warehouseArr[j]->getPrice())));
    }

}

void MainWindow::currentEdit(){
    ui->Save->setEnabled(true);

}

void MainWindow::saveWarehouse(){
    saveCurrentWarehouse();
    fillTable();
}

void MainWindow::delWarehouse(){
    if (countWarehouse){
        warehouseArr[CurrentWarehouseId] = NULL;
    qDebug() << "Ok";
        for (int i = CurrentWarehouseId; i < countWarehouse - 1; i++){
            warehouseArr[i] = warehouseArr[i + 1];
        }
    }

     warehouseArr[CurrentWarehouseId] = NULL;
     if (CurrentWarehouseId)
    CurrentWarehouseId = countWarehouse - 1 == CurrentWarehouseId && countWarehouse ? CurrentWarehouseId - 1: CurrentWarehouseId;
    countWarehouse = CurrentWarehouseId > -1 ? countWarehouse - 1 : 0;

    fillTable();

}

void MainWindow::saveCurrentWarehouse(){                     // Обработка кнопки "Сохранить 1 запись"

    Warehouse* warehouse = new Warehouse();

    switch (ui->tGroup->checkedId()) {                // Смотрим по id и сохраняем в соостветсвии с данным id
    case 0:
        warehouse->setType("bus");
        break;
    case 1:
        warehouse->setType("passenger");
        break;
    case 2:
        warehouse->setType("cargo");
        break;
    case 3:
        warehouse->setType("trailer");
        break;
    default:
        break;
    }
    warehouse->setName(ui->name->text());
    warehouse->setEC(ui->engineCapacity->value());
    warehouse->setMS(ui->maxSpeed->value());
    warehouse->setCapacity(ui->capacity->value());
    warehouse->setAcceleration(ui->accelerationTo100->value());
    warehouse->setClimContr(ui->climateControl->isChecked());

    warehouse->setLC(ui->loadCapacity->value());
    warehouse->setPrice(ui->price->value());
    QDate lastDate = warehouse->getDelivery();
    int last = warehouse->getCount();
    warehouse->setCount(ui->count->value());
    if (last != warehouse->getCount() && lastDate == (ui->delivery->date())){
        warehouse->setDelivery(QDate::currentDate());
    }                                                   // Проверка, если было изменено количество, то ставим текущую дату
    else{
        if (QDate::currentDate() >= ui->delivery->date()){
            warehouse->setDelivery(ui->delivery->date());
        }
        else{
            warehouse->setDelivery(QDate::currentDate());
        }
    }

    warehouseArr[CurrentWarehouseId] = warehouse;
}

void MainWindow::createWarehouse(){
    CurrentWarehouseId = countWarehouse;
    countWarehouse++;
    saveCurrentWarehouse();
    fillTable();
}

void MainWindow::setCurrentWarehouse(int row, int column)
{
    qDebug() << "setCurrentWarehouseId";
    qDebug() << row;
    CurrentWarehouseId = row;
    if (countWarehouse){
        if (warehouseArr[CurrentWarehouseId]->getType() != "trailer"){
            ui->engineCapacity->setEnabled(true);
            ui->maxSpeed->setEnabled(true);
            ui->capacity->setEnabled(true);
            ui->accelerationTo100->setEnabled(true);
            ui->climateControl->setEnabled(true);
        }
        else{
            ui->engineCapacity->setEnabled(false);
            ui->maxSpeed->setEnabled(false);
            ui->capacity->setEnabled(false);
            ui->accelerationTo100->setEnabled(false);
            ui->climateControl->setEnabled(false);
            ui->engineCapacity->setValue(0);
            ui->maxSpeed->setValue(0);
            ui->capacity->setValue(0);
            ui->accelerationTo100->setValue(0);
            ui->climateControl->setChecked(0);
        }

        if (warehouseArr[CurrentWarehouseId]->getType() == "bus") ui->tGroup->button(0)->setChecked(true);                   // достаём сохранённые характеристики из объекта
        if (warehouseArr[CurrentWarehouseId]->getType() == "passenger") ui->tGroup->button(1)->setChecked(true);
        if (warehouseArr[CurrentWarehouseId]->getType() == "cargo") ui->tGroup->button(2)->setChecked(true);
        if (warehouseArr[CurrentWarehouseId]->getType() == "trailer") ui->tGroup->button(3)->setChecked(true);

        ui->name->setText(warehouseArr[CurrentWarehouseId]->getName());
        ui->engineCapacity->setValue(warehouseArr[CurrentWarehouseId]->getEC());
        ui->maxSpeed->setValue(warehouseArr[CurrentWarehouseId]->getMS());
        ui->capacity->setValue(warehouseArr[CurrentWarehouseId]->getCapacity());
        ui->accelerationTo100->setValue(warehouseArr[CurrentWarehouseId]->getAcceleration());
        ui->climateControl->setChecked(warehouseArr[CurrentWarehouseId]->getClimContr());


        ui->loadCapacity->setValue(warehouseArr[CurrentWarehouseId]->getLC());
        ui->price->setValue(warehouseArr[CurrentWarehouseId]->getPrice());
        ui->count->setValue(warehouseArr[CurrentWarehouseId]->getCount());
        ui->delivery->setDate(warehouseArr[CurrentWarehouseId]->getDelivery());
    }
    ui->edit->setEnabled(true);
    ui->Delete->setEnabled(true);
    ui->Save->setEnabled(false);
}








