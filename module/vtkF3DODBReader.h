#pragma once 

#include <vtkUnstructuredGridAlgorithm.h>

class odb_FieldOutput;
class odb_String;
class odb_Odb;
class vtkDoubleArray;

class vtkF3DODBReader : public vtkUnstructuredGridAlgorithm
{
public:
  static vtkF3DODBReader* New();
  vtkTypeMacro(vtkF3DODBReader, vtkUnstructuredGridAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Get/Set the file name.
   */
  vtkSetMacro(FileName, std::string);
  vtkGetMacro(FileName, std::string);
  ///@}

protected:
  vtkF3DODBReader();
  ~vtkF3DODBReader() override;

  /**
   * Since the reader generate a vtkPolyData, we just have to derive from vtkUnstructuredGridAlgorithm
   * and override this function.
   */
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  void FillDataArray(vtkUnstructuredGrid* ug, const odb_FieldOutput& fieldOutput, const odb_Odb& odb);

  void AddPointDataByType(vtkUnstructuredGrid* ug, vtkDoubleArray* dataArray, int type);

  void AddCellDataByType(vtkUnstructuredGrid* ug, vtkDoubleArray* dataArray, int type);

private:
  vtkF3DODBReader(const vtkF3DODBReader&) = delete;
  void operator=(const vtkF3DODBReader&) = delete;

  std::string FileName;

  std::map<odb_String, std::vector<vtkIdType>> NodesMap;
  std::map<odb_String, std::vector<vtkIdType>> CellsMap;
  int NodesNum;
  int CellsNum;
};