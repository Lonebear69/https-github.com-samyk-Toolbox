//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of a library
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//==============================================================================

#include "Dictionary.h"
//------------------------------------------------------------------------------

void* DefaultOnDuplicate(const char* Name, void* Old, void* New){
  return New;
}
//------------------------------------------------------------------------------

DICTIONARY::NODE::NODE(const char* Name, void* Data){
  Red  = true;
  Left = Right = 0;

  int j;
  for(j = 0; Name[j]; j++);
  this->Name = new char[j+1];
  for(j = 0; Name[j]; j++) this->Name[j] = Name[j];
  this->Name[j] = 0;

  this->Data = Data;
}
//------------------------------------------------------------------------------

DICTIONARY::NODE::~NODE(){
  delete[] Name;

  if(Left ) delete Left;
  if(Right) delete Right;
}
//------------------------------------------------------------------------------

DICTIONARY::DICTIONARY(){
  Root        = 0;
  ItemCount   = 0;
  OnDuplicate = DefaultOnDuplicate;
}
//------------------------------------------------------------------------------

DICTIONARY::~DICTIONARY(){
  if(Root) delete Root;
}
//------------------------------------------------------------------------------

bool DICTIONARY::IsRed(NODE* Node){
  if(!Node) return false;
  return Node->Red;
}
//------------------------------------------------------------------------------

void DICTIONARY::ColourFlip(NODE* Node){
  Node       ->Red = !Node       ->Red;
  Node->Left ->Red = !Node->Left ->Red;
  Node->Right->Red = !Node->Right->Red;
}
//------------------------------------------------------------------------------

DICTIONARY::NODE* DICTIONARY::RotateLeft(NODE* Node){
  NODE* Temp  = Node->Right;
  Node->Right = Temp->Left;
  Temp->Left  = Node;
  Temp->Red   = Node->Red;
  Node->Red   = true;
  return Temp;
}
//------------------------------------------------------------------------------

DICTIONARY::NODE* DICTIONARY::RotateRight(NODE* Node){
  NODE* Temp  = Node->Left;
  Node->Left  = Temp->Right;
  Temp->Right = Node;
  Temp->Red   = Node->Red;
  Node->Red   = true;
  return Temp;
}
//------------------------------------------------------------------------------

void DICTIONARY::Insert(const char* Name, void* Data){
  Root = Insert(Root, Name, Data);
}
//------------------------------------------------------------------------------

DICTIONARY::NODE* DICTIONARY::Insert(NODE* Node, const char* Name, void* Data){
  if(!Node){
    ItemCount++;
    return new NODE(Name, Data);
  }

  int j;
  for(j = 0; Name[j]; j++){
    if(Name[j] < Node->Name[j]){
      Node->Left = Insert(Node->Left, Name, Data);
      break;

    }else if(Name[j] > Node->Name[j]){
      Node->Right = Insert(Node->Right, Name, Data);
      break;
    }
  }
  if(!Name[j]){
    if(!Node->Name[j]){
      Node->Data = OnDuplicate(Name, Node->Data, Data);
      return Node;
    }else{
      Node->Left = Insert(Node->Left, Name, Data);
    }
  }

  if(IsRed(Node->Right) && !IsRed(Node->Left      )) Node = RotateLeft (Node);
  if(IsRed(Node->Left ) &&  IsRed(Node->Left->Left)) Node = RotateRight(Node);
  if(IsRed(Node->Left ) &&  IsRed(Node->Right     ))        ColourFlip (Node);

  return Node;
}
//------------------------------------------------------------------------------

void* DICTIONARY::Find(const char* Name){
  int   j;
  NODE* Node = Root;

  while(Node){
    if(Name[0] < Node->Name[0]){
      Node = Node->Left;

    }else if(Name[0] > Node->Name[0]){
      Node = Node->Right;

    }else{
      for(j = 1; Name[j]; j++){
        if(Name[j] < Node->Name[j]){
          Node = Node->Left;
          break;

        }else if(Name[j] > Node->Name[j]){
          Node = Node->Right;
          break;
        }
      }
      if(!Name[j]){
        if(!Node->Name[j]) return Node->Data;
        else               Node = Node->Left;
      }
    }
  }
  return 0;
}
//------------------------------------------------------------------------------

int DICTIONARY::GetCount(){
  return ItemCount;
}
//------------------------------------------------------------------------------

void DICTIONARY::Action(DICTIONARY_ACTION Function){
  if(Root) Action(Root, Function);
}
//------------------------------------------------------------------------------

void DICTIONARY::Action(NODE* Node, DICTIONARY_ACTION Function){
  if(Node->Left ) Action(Node->Left , Function);
  Function(Node->Name, Node->Data);
  if(Node->Right) Action(Node->Right, Function);
}
//------------------------------------------------------------------------------
