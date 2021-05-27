/** @file LitCudaTest.h
  * @copyright Copyright (C) 2010-2011 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [orginator] **/

#ifndef LITCUDATEST_H_
#define LITCUDATEST_H_

class LitCudaTest {
public:
  LitCudaTest();
  virtual ~LitCudaTest();

  void MyDeviceInfo() const;

  void MyAddVec() const;
};

#endif /* LITCUDATEST_H_ */
