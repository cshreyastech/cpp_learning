#include<iostream>
#include <string>
#include <utility> //make_pair
#include <tuple>

struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
  int i;
};

static ShaderProgramSource ParseShaderStruct()
{
  std::string vs = "vs";
  std::string fs = "fs";
  int i = 10;

  return {vs, fs, i};
}

static std::tuple<std::string, int> ParseShaderTuple()
{
  std::string vs = "vs";
  std::string fs = "fs";
  int i = 10;
  
  // return std::make_pair(vs, fs);
  return std::make_pair(vs, i);
}

static void ParseShader(std::string* outVertexSource, std::string* outFragmentSource)
{
  std::string vs = "vs";
  std::string fs = "fs";

  if(outVertexSource)
    *outVertexSource = vs;
}

int main()
{
  std::string vs, fs;
  ParseShader(&vs, &fs);
  ParseShader(nullptr, &fs);
  
  std::tuple<std::string, int> sources = ParseShaderTuple();
  vs = std::get<0>(sources);
  int i = std::get<1>(sources);

  std::cout << vs << i << std::endl;

  ShaderProgramSource sps = ParseShaderStruct();
  

  return 0;
}