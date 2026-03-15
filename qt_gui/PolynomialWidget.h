#ifndef POLYNOMIALWIDGET_H
#define POLYNOMIALWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QComboBox>
#include <QSplitter>

#include "TPolinomManager.hpp"

class PolynomialWidget : public QWidget
{
  Q_OBJECT

public:
  explicit PolynomialWidget(TPolinomManager* manager, QWidget *parent = nullptr);

public slots:
  void createNewPolynomial();
  void addMonomial();
  void removeSelectedMonomial();
  void clearCurrentPolynomial();
  void savePolynomial();
  void loadPolynomial();
  void deletePolynomial();
  void onPolynomialSelected();
  void onMonomialChanged();
  void differentiatePolynomial();
  void integratePolynomial();
  void refreshPolynomialList();

signals:
  void polynomialChanged();

private:
  void setupUI();
  void setupConnections();
  void updatePolynomialDisplay();
  void updateMonomialList();
  void clearMonomialInputs();
  bool validateInputs();
  TMonom createMonomFromInputs();
  void displayPolynomial(const TPolinom& poly);

  QSplitter *m_mainSplitter;
  QListWidget *m_polynomialList;
  QPushButton *m_loadButton;
  QPushButton *m_deleteButton;
  QPushButton *m_refreshButton;
  QLineEdit *m_polynomialName;
  QTextEdit *m_polynomialDisplay;
  QGroupBox *m_monomialGroup;
  QDoubleSpinBox *m_coefficientSpin;
  QDoubleSpinBox *m_xPowerSpin;
  QDoubleSpinBox *m_yPowerSpin;
  QDoubleSpinBox *m_zPowerSpin;
  QPushButton *m_addMonomialButton;
  QListWidget *m_monomialList;
  QPushButton *m_removeMonomialButton;
  QPushButton *m_clearButton;
  QGroupBox *m_operationsGroup;
  QComboBox *m_variableCombo;
  QPushButton *m_differentiateButton;
  QPushButton *m_integrateButton;
  QPushButton *m_saveButton;
  QPushButton *m_newButton;
  TPolinomManager* m_manager;
  TPolinom m_currentPolynomial;
  QString m_currentPolynomialName;
  bool m_isModified;
};

#endif