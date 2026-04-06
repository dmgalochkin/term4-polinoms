#include "TableWidget.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <sstream>

TableWidget::TableWidget(TPolinomManager* manager, QWidget *parent)
  : QWidget(parent)
  , m_manager(manager)
  , m_currentTableType(0)
{
  setupUI();
  setupConnections();
  refreshTableView();
}

void TableWidget::setupUI()
{
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  QGroupBox *controlGroup = new QGroupBox("Управление таблицами");
  QGridLayout *controlLayout = new QGridLayout(controlGroup);
  controlLayout->addWidget(new QLabel("Тип таблицы:"), 0, 0);
  m_tableTypeCombo = new QComboBox();
  m_tableTypeCombo->addItems({
    "Linear Array Table",
    "Linear List Table",
    "Ordered Array Table",
    "AVL Tree Table"
  });
  controlLayout->addWidget(m_tableTypeCombo, 0, 1);
  controlLayout->addWidget(new QLabel("Поиск:"), 0, 2);
  m_searchEdit = new QLineEdit();
  m_searchEdit->setPlaceholderText("Введите имя полинома для поиска");
  controlLayout->addWidget(m_searchEdit, 0, 3);
  m_refreshButton = new QPushButton("Обновить");
  m_clearButton = new QPushButton("Очистить все");
  controlLayout->addWidget(m_refreshButton, 1, 0);
  controlLayout->addWidget(m_clearButton, 1, 1);
  mainLayout->addWidget(controlGroup);
  m_mainSplitter = new QSplitter(Qt::Vertical, this);
  QWidget *tablePanel = new QWidget();
  QVBoxLayout *tableLayout = new QVBoxLayout(tablePanel);
  tableLayout->addWidget(new QLabel("Содержимое активной таблицы:"));
  m_tableWidget = new QTableWidget();
  m_tableWidget->setColumnCount(2);
  m_tableWidget->setHorizontalHeaderLabels({"Имя", "Полином"});
  m_tableWidget->horizontalHeader()->setStretchLastSection(true);
  m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_tableWidget->setAlternatingRowColors(true);
  tableLayout->addWidget(m_tableWidget);
  QHBoxLayout *tableButtonLayout = new QHBoxLayout();
  m_removeButton = new QPushButton("Удалить выбранный");
  tableButtonLayout->addWidget(m_removeButton);
  tableButtonLayout->addStretch();
  tableLayout->addLayout(tableButtonLayout);
  mainLayout->addWidget(tablePanel);
}

void TableWidget::setupConnections()
{
  connect(m_tableTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
    this, &TableWidget::onTableTypeChanged);
  connect(m_searchEdit, &QLineEdit::textChanged, this, &TableWidget::onSearchTextChanged);
  connect(m_refreshButton, &QPushButton::clicked, this, &TableWidget::refreshTableView);
  connect(m_clearButton, &QPushButton::clicked, this, &TableWidget::clearAllPolynomials);
  connect(m_tableWidget, &QTableWidget::itemSelectionChanged, this, &TableWidget::onPolynomialSelected);
  connect(m_removeButton, &QPushButton::clicked, this, &TableWidget::removeSelectedPolynomial);
}

void TableWidget::onTableTypeChanged()
{
  int newType = m_tableTypeCombo->currentIndex();
  if (newType != m_currentTableType) {
    try {
      m_manager->SetActiveTable(newType);
      m_currentTableType = newType;
      refreshTableView();
      emit tableChanged();
    } catch (const std::exception& e) {
      QMessageBox::warning(this, "Ошибка",
        QString("Ошибка при смене таблицы: %1").arg(e.what()));
      m_tableTypeCombo->setCurrentIndex(m_currentTableType);
    }
  }
}

void TableWidget::onSearchTextChanged()
{
  QString searchText = m_searchEdit->text().trimmed();
  if (searchText.isEmpty()) {
    populateTableWidget();
    return;
  }
  try {
    auto* table = m_manager->GetActiveTable();
    TPolinom* foundPtr = table->Find(searchText.toStdString());
    if (!foundPtr) {
      m_tableWidget->setRowCount(0);
      return;
    }
    TPolinom found = *foundPtr;
    m_tableWidget->setRowCount(1);
    m_tableWidget->setItem(0, 0, new QTableWidgetItem(searchText));
    std::ostringstream oss;
    found.Print(oss);
    m_tableWidget->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(oss.str())));
  } catch (const std::exception& e) {
    m_tableWidget->setRowCount(0);
  }
}

void TableWidget::onPolynomialSelected()
{
}

void TableWidget::refreshTableView()
{
  updateTableView();
}

void TableWidget::removeSelectedPolynomial()
{
  int currentRow = m_tableWidget->currentRow();
  if (currentRow < 0) {
    QMessageBox::information(this, "Информация", "Выберите полином для удаления");
    return;
  }
  QString name = m_tableWidget->item(currentRow, 0)->text();
  int ret = QMessageBox::question(this, "Подтверждение",
    QString("Удалить полином '%1' из активной таблицы?").arg(name),
    QMessageBox::Yes | QMessageBox::No);
  if (ret == QMessageBox::Yes) {
    try {
      m_manager->RemovePolinom(name.toStdString());
      refreshTableView();
      QMessageBox::information(this, "Успех",
        QString("Полином '%1' удален").arg(name));
      emit tableChanged();
    } catch (const std::exception& e) {
      QMessageBox::warning(this, "Ошибка",
        QString("Ошибка при удалении: %1").arg(e.what()));
    }
  }
}

void TableWidget::clearAllPolynomials()
{
  int ret = QMessageBox::question(this, "Подтверждение",
    "Очистить все полиномы из активной таблицы?",
    QMessageBox::Yes | QMessageBox::No);
  if (ret == QMessageBox::Yes) {
    try {
      m_manager->GetActiveTable()->Clear();
      refreshTableView();
      QMessageBox::information(this, "Успех", "Активная таблица очищена");
      emit tableChanged();
    } catch (const std::exception& e) {
      QMessageBox::warning(this, "Ошибка",
        QString("Ошибка при очистке: %1").arg(e.what()));
    }
  }
}

void TableWidget::updateTableView()
{
  populateTableWidget();
}

void TableWidget::populateTableWidget()
{
  try {
    auto* table = m_manager->GetActiveTable();
    auto names = table->GetKeys();
    m_tableWidget->setRowCount(names.size());
    int row = 0;
    for (const auto& name : names) {
      try {
        TPolinom* polyPtr = table->Find(name);
        if (!polyPtr) continue;
        TPolinom poly = *polyPtr;
        m_tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(name)));
        std::ostringstream oss;
        poly.Print(oss);
        m_tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(oss.str())));
        row++;
      } catch (const std::exception& e) {
        continue;
      }
    }
    if (row < static_cast<int>(names.size())) {
      m_tableWidget->setRowCount(row);
    }
  } catch (const std::exception& e) {
    m_tableWidget->setRowCount(0);
    QMessageBox::warning(this, "Ошибка",
      QString("Ошибка при загрузке таблицы: %1").arg(e.what()));
  }
}

QString TableWidget::getTableTypeName(int index)
{
  switch (index) 
  {
    case 0: return "Linear Array Table";
    case 1: return "Linear List Table";
    case 2: return "Ordered Array Table";
    case 3: return "AVL Tree Table";
    case 4: return "Hash Table";
    case 5: return "Red-Black Tree Table";
    default: return "Unknown";
  }
}

