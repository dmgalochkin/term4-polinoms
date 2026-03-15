#include "ExpressionWidget.h"
#include <QMessageBox>
#include <QInputDialog>
#include <sstream>

ExpressionWidget::ExpressionWidget(TPolinomManager* manager, QWidget *parent)
  : QWidget(parent)
  , m_manager(manager)
{
  setupUI();
  setupConnections();
}

void ExpressionWidget::setupUI()
{
  m_mainSplitter = new QSplitter(Qt::Horizontal, this);
  QWidget *leftPanel = new QWidget();
  QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
  leftLayout->addWidget(new QLabel("Введите выражение:"));
  m_expressionEdit = new QLineEdit();
  m_expressionEdit->setPlaceholderText("Например: pol1 + pol2 * 2.5 - 3.0");
  leftLayout->addWidget(m_expressionEdit);
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  m_evaluateButton = new QPushButton("Вычислить");
  m_evaluateAndSaveButton = new QPushButton("Вычислить и сохранить");
  m_clearButton = new QPushButton("Очистить");
  m_helpButton = new QPushButton("Справка");
  buttonLayout->addWidget(m_evaluateButton);
  buttonLayout->addWidget(m_evaluateAndSaveButton);
  buttonLayout->addWidget(m_clearButton);
  buttonLayout->addWidget(m_helpButton);
  leftLayout->addLayout(buttonLayout);
  leftLayout->addWidget(new QLabel("Результат:"));
  m_resultDisplay = new QTextEdit();
  m_resultDisplay->setReadOnly(true);
  m_resultDisplay->setMaximumHeight(120);
  m_resultDisplay->setPlainText("Результат появится здесь...");
  leftLayout->addWidget(m_resultDisplay);
  QWidget *rightPanel = new QWidget();
  QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
  rightLayout->addWidget(new QLabel("История вычислений:"));
  m_historyList = new QListWidget();
  rightLayout->addWidget(m_historyList);
  m_clearHistoryButton = new QPushButton("Очистить историю");
  rightLayout->addWidget(m_clearHistoryButton);
  rightPanel->setMaximumWidth(300);
  m_mainSplitter->addWidget(leftPanel);
  m_mainSplitter->addWidget(rightPanel);
  m_mainSplitter->setSizes({500, 300});
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(m_mainSplitter);
}

void ExpressionWidget::setupConnections()
{
  connect(m_evaluateButton, &QPushButton::clicked, this, &ExpressionWidget::evaluateExpression);
  connect(m_evaluateAndSaveButton, &QPushButton::clicked, this, &ExpressionWidget::evaluateAndSave);
  connect(m_clearButton, &QPushButton::clicked, this, &ExpressionWidget::clearExpression);
  connect(m_helpButton, &QPushButton::clicked, this, &ExpressionWidget::showExpressionHelp);
  connect(m_historyList, &QListWidget::itemClicked, this, &ExpressionWidget::onHistoryItemSelected);
  connect(m_clearHistoryButton, &QPushButton::clicked, this, &ExpressionWidget::clearHistory);
  connect(m_expressionEdit, &QLineEdit::textChanged, this, &ExpressionWidget::onExpressionChanged);
  connect(m_expressionEdit, &QLineEdit::returnPressed, this, &ExpressionWidget::evaluateExpression);
}

void ExpressionWidget::evaluateExpression()
{
  QString expression = m_expressionEdit->text().trimmed();
  if (expression.isEmpty()) {
    return;
  }
  try {
    TPostfixCalculator calculator(m_manager->GetActiveTable());
    TPolinom result = calculator.Evaluate(expression.toStdString());
    QString resultStr = formatResult(result);
    m_resultDisplay->setPlainText(resultStr);
    addToHistory(expression, resultStr);
    emit expressionEvaluated(expression, resultStr);
  } catch (const std::exception& e) {
    QString error = QString("Ошибка: %1").arg(e.what());
    m_resultDisplay->setPlainText(error);
    QMessageBox::warning(this, "Ошибка вычисления", error);
  }
}

void ExpressionWidget::evaluateAndSave()
{
  QString expression = m_expressionEdit->text().trimmed();
  if (expression.isEmpty()) {
    QMessageBox::warning(this, "Ошибка", "Введите выражение для вычисления");
    return;
  }
  bool ok;
  QString saveName = QInputDialog::getText(this, "Сохранение результата",
    "Введите имя для сохранения результата:", QLineEdit::Normal,
    "result_poly", &ok);
  if (!ok || saveName.trimmed().isEmpty()) {
    return;
  }
  saveName = saveName.trimmed();
  try {
    TPostfixCalculator calculator(m_manager->GetActiveTable());
    TPolinom result = calculator.Evaluate(expression.toStdString());
    m_manager->AddPolinom(saveName.toStdString(), result);
    QString resultStr = formatResult(result);
    m_resultDisplay->setPlainText(resultStr);
    addToHistory(expression + " → " + saveName, resultStr);
    QMessageBox::information(this, "Успех",
      QString("Результат вычисления сохранен как '%1'").arg(saveName));
    emit expressionEvaluated(expression, resultStr);
  } catch (const std::exception& e) {
    QString error = QString("Ошибка: %1").arg(e.what());
    m_resultDisplay->setPlainText(error);
    QMessageBox::warning(this, "Ошибка", error);
  }
}

void ExpressionWidget::clearExpression()
{
  m_expressionEdit->clear();
  m_resultDisplay->setPlainText("Результат появится здесь...");
}

void ExpressionWidget::clearHistory()
{
  m_historyList->clear();
  m_expressionHistory.clear();
  m_resultHistory.clear();
}

void ExpressionWidget::onExpressionChanged()
{
}

void ExpressionWidget::onHistoryItemSelected()
{
  QListWidgetItem* item = m_historyList->currentItem();
  if (item) {
    QString historyText = item->text();
    int equalPos = historyText.indexOf('=');
    if (equalPos > 0) {
      QString expression = historyText.left(equalPos).trimmed();
      int arrowPos = expression.indexOf(QString::fromUtf8("→"));
      if (arrowPos > 0) {
        expression = expression.left(arrowPos).trimmed();
      }
      m_expressionEdit->setText(expression);
    }
  }
}

void ExpressionWidget::showExpressionHelp()
{
  QString helpText =
    "<h3>Справка по выражениям</h3>"
    "<p><b>Поддерживаемые операции:</b></p>"
    "<ul>"
    "<li><b>+</b> - сложение полиномов</li>"
    "<li><b>-</b> - вычитание полиномов</li>"
    "<li><b>*</b> - умножение полиномов или на константу</li>"
    "<li><b>( )</b> - группировка операций</li>"
    "</ul>"
    "<p><b>Примеры выражений:</b></p>"
    "<ul>"
    "<li><code>pol1 + pol2</code> - сложение двух полиномов</li>"
    "<li><code>pol1 * 2.5</code> - умножение на константу</li>"
    "<li><code>(pol1 + pol2) * pol3</code> - сложные выражения</li>"
    "<li><code>pol1 - pol2 + 3.14</code> - смешанные операции</li>"
    "</ul>"
    "<p><b>Примечания:</b></p>"
    "<ul>"
    "<li>Имена полиномов должны существовать в активной таблице</li>"
    "<li>Константы записываются как десятичные числа (например: 2.5, -1.0)</li>"
    "<li>Пробелы вокруг операторов не обязательны</li>"
    "<li>Результат можно сохранить под новым именем</li>"
    "</ul>";
  QMessageBox::information(this, "Справка по выражениям", helpText);
}

void ExpressionWidget::addToHistory(const QString& expression, const QString& result)
{
  QString historyItem = QString("%1 = %2").arg(expression, result);
  m_historyList->insertItem(0, historyItem);
  m_expressionHistory.prepend(expression);
  m_resultHistory.prepend(result);
  const int maxHistorySize = 50;
  while (m_historyList->count() > maxHistorySize) {
    delete m_historyList->takeItem(m_historyList->count() - 1);
    m_expressionHistory.removeLast();
    m_resultHistory.removeLast();
  }
}

QString ExpressionWidget::formatResult(const TPolinom& result)
{
  std::ostringstream oss;
  result.Print(oss);
  return QString::fromStdString(oss.str());
}
