#include "PolynomialWidget.h"
#include <QMessageBox>
#include <QInputDialog>
#include <sstream>

PolynomialWidget::PolynomialWidget(TPolinomManager* manager, QWidget *parent)
  : QWidget(parent)
  , m_manager(manager)
  , m_currentPolynomial(3)
  , m_isModified(false)
{
  setupUI();
  setupConnections();
  refreshPolynomialList();
}

void PolynomialWidget::setupUI()
{
  m_mainSplitter = new QSplitter(Qt::Horizontal, this);
  QWidget *leftPanel = new QWidget();
  QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
  leftLayout->addWidget(new QLabel("Сохраненные полиномы:"));
  m_polynomialList = new QListWidget();
  leftLayout->addWidget(m_polynomialList);
  QHBoxLayout *leftButtonLayout = new QHBoxLayout();
  m_loadButton = new QPushButton("Загрузить");
  m_deleteButton = new QPushButton("Удалить");
  m_refreshButton = new QPushButton("Обновить");
  leftButtonLayout->addWidget(m_loadButton);
  leftButtonLayout->addWidget(m_deleteButton);
  leftButtonLayout->addWidget(m_refreshButton);
  leftLayout->addLayout(leftButtonLayout);
  leftPanel->setMaximumWidth(250);
  QWidget *rightPanel = new QWidget();
  QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
  QHBoxLayout *nameLayout = new QHBoxLayout();
  nameLayout->addWidget(new QLabel("Имя полинома:"));
  m_polynomialName = new QLineEdit();
  m_polynomialName->setPlaceholderText("Введите имя полинома");
  nameLayout->addWidget(m_polynomialName);
  rightLayout->addLayout(nameLayout);
  rightLayout->addWidget(new QLabel("Текущий полином:"));
  m_polynomialDisplay = new QTextEdit();
  m_polynomialDisplay->setReadOnly(true);
  m_polynomialDisplay->setMaximumHeight(100);
  m_polynomialDisplay->setPlainText("0");
  rightLayout->addWidget(m_polynomialDisplay);
  m_monomialGroup = new QGroupBox("Добавить моном");
  QGridLayout *monomialLayout = new QGridLayout(m_monomialGroup);
  monomialLayout->addWidget(new QLabel("Коэффициент:"), 0, 0);
  m_coefficientSpin = new QDoubleSpinBox();
  m_coefficientSpin->setRange(-1000.0, 1000.0);
  m_coefficientSpin->setDecimals(3);
  m_coefficientSpin->setValue(1.0);
  monomialLayout->addWidget(m_coefficientSpin, 0, 1);
  monomialLayout->addWidget(new QLabel("Степень x:"), 1, 0);
  m_xPowerSpin = new QDoubleSpinBox();
  m_xPowerSpin->setRange(-10.0, 10.0);
  m_xPowerSpin->setDecimals(2);
  monomialLayout->addWidget(m_xPowerSpin, 1, 1);
  monomialLayout->addWidget(new QLabel("Степень y:"), 2, 0);
  m_yPowerSpin = new QDoubleSpinBox();
  m_yPowerSpin->setRange(-10.0, 10.0);
  m_yPowerSpin->setDecimals(2);
  monomialLayout->addWidget(m_yPowerSpin, 2, 1);
  monomialLayout->addWidget(new QLabel("Степень z:"), 3, 0);
  m_zPowerSpin = new QDoubleSpinBox();
  m_zPowerSpin->setRange(-10.0, 10.0);
  m_zPowerSpin->setDecimals(2);
  monomialLayout->addWidget(m_zPowerSpin, 3, 1);
  m_addMonomialButton = new QPushButton("Добавить моном");
  monomialLayout->addWidget(m_addMonomialButton, 4, 0, 1, 2);
  rightLayout->addWidget(m_monomialGroup);
  rightLayout->addWidget(new QLabel("Мономы в полиноме:"));
  m_monomialList = new QListWidget();
  m_monomialList->setMaximumHeight(120);
  rightLayout->addWidget(m_monomialList);
  QHBoxLayout *monomialButtonLayout = new QHBoxLayout();
  m_removeMonomialButton = new QPushButton("Удалить моном");
  m_clearButton = new QPushButton("Очистить все");
  monomialButtonLayout->addWidget(m_removeMonomialButton);
  monomialButtonLayout->addWidget(m_clearButton);
  rightLayout->addLayout(monomialButtonLayout);
  m_operationsGroup = new QGroupBox("Операции с полиномом");
  QGridLayout *operationsLayout = new QGridLayout(m_operationsGroup);
  operationsLayout->addWidget(new QLabel("Переменная:"), 0, 0);
  m_variableCombo = new QComboBox();
  m_variableCombo->addItems({"x (0)", "y (1)", "z (2)"});
  operationsLayout->addWidget(m_variableCombo, 0, 1);
  m_differentiateButton = new QPushButton("Дифференцировать");
  operationsLayout->addWidget(m_differentiateButton, 1, 0);
  m_integrateButton = new QPushButton("Интегрировать");
  operationsLayout->addWidget(m_integrateButton, 1, 1);
  rightLayout->addWidget(m_operationsGroup);
  QHBoxLayout *controlLayout = new QHBoxLayout();
  m_newButton = new QPushButton("Новый полином");
  m_saveButton = new QPushButton("Сохранить полином");
  controlLayout->addWidget(m_newButton);
  controlLayout->addWidget(m_saveButton);
  rightLayout->addLayout(controlLayout);
  m_mainSplitter->addWidget(leftPanel);
  m_mainSplitter->addWidget(rightPanel);
  m_mainSplitter->setSizes({250, 550});
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(m_mainSplitter);
}

void PolynomialWidget::setupConnections()
{
  connect(m_loadButton, &QPushButton::clicked, this, &PolynomialWidget::loadPolynomial);
  connect(m_deleteButton, &QPushButton::clicked, this, &PolynomialWidget::deletePolynomial);
  connect(m_refreshButton, &QPushButton::clicked, this, &PolynomialWidget::refreshPolynomialList);
  connect(m_polynomialList, &QListWidget::itemSelectionChanged, this, &PolynomialWidget::onPolynomialSelected);
  connect(m_addMonomialButton, &QPushButton::clicked, this, &PolynomialWidget::addMonomial);
  connect(m_removeMonomialButton, &QPushButton::clicked, this, &PolynomialWidget::removeSelectedMonomial);
  connect(m_clearButton, &QPushButton::clicked, this, &PolynomialWidget::clearCurrentPolynomial);
  connect(m_newButton, &QPushButton::clicked, this, &PolynomialWidget::createNewPolynomial);
  connect(m_saveButton, &QPushButton::clicked, this, &PolynomialWidget::savePolynomial);
  connect(m_differentiateButton, &QPushButton::clicked, this, &PolynomialWidget::differentiatePolynomial);
  connect(m_integrateButton, &QPushButton::clicked, this, &PolynomialWidget::integratePolynomial);
  connect(m_polynomialName, &QLineEdit::textChanged, this, &PolynomialWidget::onMonomialChanged);
  connect(m_coefficientSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PolynomialWidget::onMonomialChanged);
}

void PolynomialWidget::createNewPolynomial()
{
  m_currentPolynomial = TPolinom(3);
  m_currentPolynomialName.clear();
  m_polynomialName->clear();
  m_isModified = false;
  clearMonomialInputs();
  updatePolynomialDisplay();
  updateMonomialList();
}

void PolynomialWidget::addMonomial()
{
  if (!validateInputs()) {
    return;
  }
  try {
    TMonom monom = createMonomFromInputs();
    m_currentPolynomial = m_currentPolynomial + TPolinom(monom);
    m_isModified = true;
    updatePolynomialDisplay();
    updateMonomialList();
    clearMonomialInputs();
    emit polynomialChanged();
  } catch (const std::exception& e) {
    QMessageBox::warning(this, "Ошибка", QString("Ошибка при добавлении монома: %1").arg(e.what()));
  }
}

void PolynomialWidget::removeSelectedMonomial()
{
  int currentRow = m_monomialList->currentRow();
  if (currentRow < 0) {
    QMessageBox::information(this, "Информация", "Выберите моном для удаления");
    return;
  }
  QMessageBox::information(this, "Информация", "Функция удаления отдельного монома будет реализована");
}

void PolynomialWidget::clearCurrentPolynomial()
{
  m_currentPolynomial = TPolinom(3);
  m_isModified = true;
  updatePolynomialDisplay();
  updateMonomialList();
  emit polynomialChanged();
}

void PolynomialWidget::savePolynomial()
{
  QString name = m_polynomialName->text().trimmed();
  if (name.isEmpty()) {
    QMessageBox::warning(this, "Ошибка", "Введите имя полинома");
    return;
  }
  try {
    m_manager->AddPolinom(name.toStdString(), m_currentPolynomial);
    m_currentPolynomialName = name;
    m_isModified = false;
    refreshPolynomialList();
    QMessageBox::information(this, "Успех", QString("Полином '%1' сохранен").arg(name));
    emit polynomialChanged();
  } catch (const std::exception& e) {
    QMessageBox::warning(this, "Ошибка", QString("Ошибка при сохранении: %1").arg(e.what()));
  }
}

void PolynomialWidget::loadPolynomial()
{
  QListWidgetItem* item = m_polynomialList->currentItem();
  if (!item) {
    QMessageBox::information(this, "Информация", "Выберите полином для загрузки");
    return;
  }
  QString name = item->text();
  try {
    auto* table = m_manager->GetActiveTable();
    TPolinom* polyPtr = table->Find(name.toStdString());
    if (!polyPtr) {
      QMessageBox::warning(this, "Ошибка", QString("Полином '%1' не найден").arg(name));
      return;
    }
    TPolinom poly = *polyPtr;
    m_currentPolynomial = poly;
    m_currentPolynomialName = name;
    m_polynomialName->setText(name);
    m_isModified = false;
    updatePolynomialDisplay();
    updateMonomialList();
    QMessageBox::information(this, "Успех", QString("Полином '%1' загружен").arg(name));
  } catch (const std::exception& e) {
    QMessageBox::warning(this, "Ошибка", QString("Ошибка при загрузке: %1").arg(e.what()));
  }
}

void PolynomialWidget::deletePolynomial()
{
  QListWidgetItem* item = m_polynomialList->currentItem();
  if (!item) {
    QMessageBox::information(this, "Информация", "Выберите полином для удаления");
    return;
  }
  QString name = item->text();
  int ret = QMessageBox::question(this, "Подтверждение", 
    QString("Удалить полином '%1'?").arg(name),
    QMessageBox::Yes | QMessageBox::No);
  if (ret == QMessageBox::Yes) {
    try {
      m_manager->RemovePolinom(name.toStdString());
      refreshPolynomialList();
      if (m_currentPolynomialName == name) {
        createNewPolynomial();
      }
      QMessageBox::information(this, "Успех", QString("Полином '%1' удален").arg(name));
      emit polynomialChanged();
    } catch (const std::exception& e) {
      QMessageBox::warning(this, "Ошибка", QString("Ошибка при удалении: %1").arg(e.what()));
    }
  }
}

void PolynomialWidget::onPolynomialSelected()
{
}

void PolynomialWidget::onMonomialChanged()
{
  m_isModified = true;
}

void PolynomialWidget::differentiatePolynomial()
{
  int varIndex = m_variableCombo->currentIndex();
  try {
    TPolinom result = m_currentPolynomial.Differentiate(varIndex);
    m_currentPolynomial = result;
    m_isModified = true;
    updatePolynomialDisplay();
    updateMonomialList();
    QString varName = (varIndex == 0) ? "x" : (varIndex == 1) ? "y" : "z";
    QMessageBox::information(this, "Успех", QString("Полином продифференцирован по переменной %1").arg(varName));
    emit polynomialChanged();
  } catch (const std::exception& e) {
    QMessageBox::warning(this, "Ошибка", QString("Ошибка при дифференцировании: %1").arg(e.what()));
  }
}

void PolynomialWidget::integratePolynomial()
{
  int varIndex = m_variableCombo->currentIndex();
  try {
    TPolinom result = m_currentPolynomial.Integrate(varIndex);
    m_currentPolynomial = result;
    m_isModified = true;
    updatePolynomialDisplay();
    updateMonomialList();
    QString varName = (varIndex == 0) ? "x" : (varIndex == 1) ? "y" : "z";
    QMessageBox::information(this, "Успех", QString("Полином проинтегрирован по переменной %1").arg(varName));
    emit polynomialChanged();
  } catch (const std::exception& e) {
    QMessageBox::warning(this, "Ошибка", QString("Ошибка при интегрировании: %1").arg(e.what()));
  }
}

void PolynomialWidget::refreshPolynomialList()
{
  m_polynomialList->clear();
  try {
    auto* table = m_manager->GetActiveTable();
    auto names = table->GetKeys();
    for (const auto& name : names) {
      m_polynomialList->addItem(QString::fromStdString(name));
    }
  } catch (const std::exception& e) {
    QMessageBox::warning(this, "Ошибка", QString("Ошибка при обновлении списка: %1").arg(e.what()));
  }
}

void PolynomialWidget::updatePolynomialDisplay()
{
  std::ostringstream oss;
  m_currentPolynomial.Print(oss);
  m_polynomialDisplay->setPlainText(QString::fromStdString(oss.str()));
}

void PolynomialWidget::updateMonomialList()
{
  m_monomialList->clear();
  std::ostringstream oss;
  m_currentPolynomial.Print(oss);
  QString polyStr = QString::fromStdString(oss.str());
  if (polyStr != "0") {
    QStringList parts = polyStr.split('+');
    for (const QString& part : parts) {
      m_monomialList->addItem(part.trimmed());
    }
  }
}

void PolynomialWidget::clearMonomialInputs()
{
  m_coefficientSpin->setValue(1.0);
  m_xPowerSpin->setValue(0.0);
  m_yPowerSpin->setValue(0.0);
  m_zPowerSpin->setValue(0.0);
}

bool PolynomialWidget::validateInputs()
{
  if (m_coefficientSpin->value() == 0.0) {
    QMessageBox::warning(this, "Ошибка", "Коэффициент не может быть равен нулю");
    return false;
  }
  return true;
}

TMonom PolynomialWidget::createMonomFromInputs()
{
  double coeff = m_coefficientSpin->value();
  std::vector<double> powers = {
    m_xPowerSpin->value(),
    m_yPowerSpin->value(),
    m_zPowerSpin->value()
  };
  return TMonom(coeff, powers);
}

void PolynomialWidget::displayPolynomial(const TPolinom& poly)
{
  std::ostringstream oss;
  poly.Print(oss);
  m_polynomialDisplay->setPlainText(QString::fromStdString(oss.str()));
}
