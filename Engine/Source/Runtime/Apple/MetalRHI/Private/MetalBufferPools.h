// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.


#pragma once

#import <Metal/Metal.h>

struct FMetalPooledBufferArgs
{
	FMetalPooledBufferArgs() : Device(nil), Size(0), Storage(MTLStorageModeShared) {}
	
	FMetalPooledBufferArgs(id<MTLDevice> InDevice, uint32 InSize, MTLStorageMode InStorage)
	: Device(InDevice)
	, Size(InSize)
	, Storage(InStorage)
	{
	}
	
	id<MTLDevice> Device;
	uint32 Size;
	MTLStorageMode Storage;
};

struct FMetalQueryBufferPool
{
    enum
    {
        EQueryBufferAlignment = 8,
        EQueryResultMaxSize = 8,
        EQueryBufferMaxSize = 64 * 1024
    };
    
    FMetalQueryBufferPool(class FMetalContext* InContext)
    : Context(InContext)
    {
    }
    
    void Allocate(FMetalQueryResult& NewQuery);
    FMetalQueryBuffer* GetCurrentQueryBuffer();
    void ReleaseQueryBuffer(id<MTLBuffer> Buffer);
    
    FMetalQueryBufferRef CurrentBuffer;
    TArray<id<MTLBuffer>> Buffers;
	class FMetalContext* Context;
};

struct FRingBuffer
{
	FRingBuffer(id<MTLDevice> Device, MTLResourceOptions Options, uint32 Size, uint32 InDefaultAlignment);

	uint32 GetOffset() { return Offset; }
	void SetLastRead(uint32 Read) { FPlatformAtomics::InterlockedExchange((int32*)&LastRead, Read); }
	uint32 Allocate(uint32 Size, uint32 Alignment);
	void Shrink();

	uint32 FrameSize[10];
	uint64 LastFrameChange;
	MTLResourceOptions Options;
	uint32 InitialSize;
	uint32 DefaultAlignment;
	id<MTLBuffer> Buffer;
	uint32 Offset;
	uint32 LastRead;
	uint32 LastWritten;
};
