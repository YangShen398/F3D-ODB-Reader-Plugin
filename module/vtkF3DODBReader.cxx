#include "vtkF3DODBReader.h"
#include "vtkAbqIncludes.h"

#include <vtkUnstructuredGrid.h>
#include <vtkType.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

int ABAQUS_VTK_CELL_MAP(const char* abqElementType)
{
  std::string type(abqElementType);
  if (type.find("C3D4") != std::string::npos)
  {
    return 10;
  }
  else if (type.find("C3D6") != std::string::npos)
  {
    return 13;
  }
  else if (type.find("C3D8") != std::string::npos)
  {
    return 12;
  }
  else if (type.find("C3D10") != std::string::npos)
  {
    return 24;
  }
  else if (type.find("C3D15") != std::string::npos)
  {
    return 26;
  }
  else if (type.find("C3D20") != std::string::npos)
  {
    return 25;
  }
  else if (type.find("S3") != std::string::npos)
  {
    return 5;
  }
  else if (type.find("S4") != std::string::npos)
  {
    return 9;
  }
  else if (type.find("S8") != std::string::npos)
  {
    return 23;
  }
  else if (type.find("S9") != std::string::npos)
  {
    return 28;
  }
  else if (type.find("B31") != std::string::npos)
  {
    return 3;
  }
  else if (type.find("R3D3") != std::string::npos)
  {
    return 5;
  }
  else if (type.find("R3D4") != std::string::npos)
  {
    return 9;
  }

  std::cerr << type << " not supported by the converter." << std::endl;
  return -1;
}

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkF3DODBReader);

//----------------------------------------------------------------------------
vtkF3DODBReader::vtkF3DODBReader()
{
  // All the readers have no input, this have to be specified to the VTK pipeline
  this->SetNumberOfInputPorts(0);
}

//----------------------------------------------------------------------------
vtkF3DODBReader::~vtkF3DODBReader() = default;

//----------------------------------------------------------------------------
void vtkF3DODBReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "FileName: " << (this->FileName.empty() ? "(none)" : this->FileName) << "\n";
}

//----------------------------------------------------------------------------
int vtkF3DODBReader::RequestData(
  vtkInformation*, vtkInformationVector**, vtkInformationVector* outputVector)
{
  odb_initializeAPI();
  
  try {

    // vtkPolyData is the VTK class of surface mesh
    // Other data structure exists for volumetric mesh like vtkImageData or vtkUnstructuredGrid
    vtkUnstructuredGrid* output = vtkUnstructuredGrid::GetData(outputVector);
    vtkNew<vtkPoints> points;
    vtkNew<vtkCellArray> cellArray;
    std::vector<int> cellTypes;

    odb_String odbFile = odb_String(this->FileName.c_str());
    auto odb = &openOdb(odbFile);
    odb_InstanceRepositoryIT instIter(odb->rootAssembly().instances());

    this->NodesNum = 0;
    this->CellsNum = 0;
    for (instIter.first(); !instIter.isDone(); instIter.next())
    {
      std::cout << instIter.currentKey().cStr() << std::endl;
      auto instName = instIter.currentKey();
      auto nodes = instIter.currentValue().nodes();
      auto cells = instIter.currentValue().elements();

      // instantiate two maps from abaqus label (label - 1 because label starts from 1) to vtk index
      this->NodesMap[instName] = std::vector<vtkIdType>(nodes.size(), 0);
      this->CellsMap[instName] = std::vector<vtkIdType>(cells.size(), 0);
      this->NodesNum += nodes.size();
      this->CellsNum += cells.size();
      // insert nodes to vtkPoints and put global index in the map
      for (int i = 0; i < nodes.size(); i++) 
      {
        this->NodesMap[instName][i] = points->InsertNextPoint(nodes[i].coordinates());
      }
      // insert cell to vtkCellArray and put global index in the map
      for (int i = 0; i < cells.size(); i++)
      {
        auto cellType = ABAQUS_VTK_CELL_MAP(cells[i].type().CStr());
        auto conn = cells[i].connectivity();
        int numOfNodesInCell = conn.Length();
        vtkIdType* ids = new vtkIdType[numOfNodesInCell];
        for (int id = 0; id < numOfNodesInCell; id++) 
        {
          // conn[id] is the node label which starts from 1
          ids[id] = this->NodesMap[instName][conn[id] - 1];
        }
        this->CellsMap[instName][i] = cellArray->InsertNextCell(numOfNodesInCell, ids);
        cellTypes.push_back(cellType);
        delete [] ids;
      }
    }

    output->SetPoints(points);
    output->SetCells(cellTypes.data(), cellArray);

    // only export the last frame of the last step
    auto frames = odb->steps().stepList().Last().frames();
    int numOfFrames = frames.size();
    auto fieldOutputs = frames[numOfFrames - 1].fieldOutputs();
    
    odb_FieldOutputRepositoryIT fieldIter(fieldOutputs);
    for (fieldIter.first(); !fieldIter.isDone(); fieldIter.next())
    {
      auto fldOuput = fieldIter.currentValue();
      this->FillDataArray(output, fldOuput, *odb);
    }

    odb->close();
  }
  catch (odb_BaseException& exc) {
        std::cout << "Abaqus error message: " << exc.UserReport().CStr() << std::endl;
  }
  catch (...) {
      std::cout << "Unknown Exception.\n";
  }

  odb_finalizeAPI();

  return 1;
}

void vtkF3DODBReader::FillDataArray(vtkUnstructuredGrid* ug, const odb_FieldOutput& fieldOutput, const odb_Odb& odb)
{
  vtkNew<vtkDoubleArray> dataArray;
  std::cout << fieldOutput.name().cStr() << std::endl;
  dataArray->SetName(fieldOutput.name().cStr());
  auto abqPos = fieldOutput.locations().constGet(0).position();
  auto abqComponentLabels = fieldOutput.componentLabels();
  int componentSize = abqComponentLabels.Length() == 0 ? 1 : abqComponentLabels.Length();
  dataArray->SetNumberOfComponents(componentSize);
  if (abqPos == odb_Enum::odb_ResultPositionEnum::NODAL) 
  {
    // write point data
    dataArray->SetNumberOfTuples(this->NodesNum);
    dataArray->FillComponent(0, 0);
    for (int cp = 0; cp < abqComponentLabels.Length(); cp++)
    {
      dataArray->SetComponentName(cp, abqComponentLabels[cp].cStr());
    }
    for (const auto& item : this->NodesMap)
    {
      auto inst = odb.constRootAssembly().instances().constGet(item.first);
      auto fldOutputInst = fieldOutput.getSubset(inst);
      for (int i = 0; i < fldOutputInst.bulkDataBlocks().size(); i++)
      {
        auto blkData = fldOutputInst.bulkDataBlocks()[i];
        int numNodesInBulk = blkData.length();
        int numCompInBulk = blkData.width();
        float* data = blkData.data();
        for (int j = 0; j < numNodesInBulk; j++)
        {
          int nodeLabelABQ = blkData.nodeLabels()[j];
          vtkIdType nodeIndexVTK = item.second[nodeLabelABQ - 1];
          dataArray->SetTuple(nodeIndexVTK, data + j * numCompInBulk); 
        }
      }     
    }
    ug->GetPointData()->AddArray(dataArray);
  }
  else if (abqPos == odb_Enum::odb_ResultPositionEnum::INTEGRATION_POINT)
  {
    // write cell data
    dataArray->SetNumberOfTuples(this->CellsNum);
    dataArray->FillComponent(0, 0);
    for (int cp = 0; cp < abqComponentLabels.Length(); cp++)
    {
      dataArray->SetComponentName(cp, abqComponentLabels[cp].cStr());
    }
    for (const auto& item : this->CellsMap)
    {
      auto inst = odb.constRootAssembly().instances().constGet(item.first);
      auto fldOutputInst = fieldOutput.getSubset(inst).getSubset(odb_Enum::odb_ResultPositionEnum::CENTROID);
      for (int i = 0; i < fldOutputInst.bulkDataBlocks().size(); i++)
      {
        auto blkData = fldOutputInst.bulkDataBlocks()[i];
        int numOfElementsInBulk = blkData.length();
        int numOfCompInBulk = blkData.width();
        float* data = blkData.data();
        for (int j = 0; j < numOfElementsInBulk; j++)
        {
          int nodeLabelABQ = blkData.elementLabels()[j];
          vtkIdType nodeIndexVTK = item.second[nodeLabelABQ - 1];
          dataArray->SetTuple(nodeIndexVTK, data + j * numOfCompInBulk); 
        }
      }     
    }
    ug->GetCellData()->AddArray(dataArray);
  }
}
