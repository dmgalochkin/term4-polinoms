#ifndef EXPRESSIONWIDGET_H
#define EXPRESSIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QListWidget>
#include <QGroupBox>
#include <QSplitter>
#include <QComboBox>
#include <QCheckBox>

#include "TPolinomManager.hpp"

class ExpressionWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ExpressionWidget(TPolinomManager* manager, QWidget *parent = nullptr);

public slots:
  void evaluateExpression();
  void evaluateAndSave();
  void clearExpression();
  void clearHistory();
  void onExpressionChanged();
  void onHistoryItemSelected();
  void showExpressionHelp();

signals:
  void expressionEvaluated(const QString& expression, const QString& result);

private:
  void setupUI();
  void setupConnections();
  void addToHistory(const QString& expression, const QString& result);
  void validateExpression();
  QString formatResult(const TPolinom& result);

  QSplitter *m_mainSplitter;
  QLineEdit *m_expressionEdit;
  QPushButton *m_evaluateButton;
  QPushButton *m_evaluateAndSaveButton;
  QPushButton *m_clearButton;
  QPushButton *m_helpButton;
  QTextEdit *m_resultDisplay;
  QListWidget *m_historyList;
  QPushButton *m_clearHistoryButton;
  TPolinomManager* m_manager;
  QStringList m_expressionHistory;
  QStringList m_resultHistory;
};

#endif