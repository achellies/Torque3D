//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "platformX86UNIX/platformX86UNIX.h"
#include "platform/threads/semaphore.h"
// Instead of that mess that was here before, lets use the SDL lib to deal
// with the semaphores.

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

struct PlatformSemaphore
{
   SDL_sem *semaphore;

   PlatformSemaphore(S32 initialCount)
   {
       semaphore = SDL_CreateSemaphore(initialCount);
       AssertFatal(semaphore, "PlatformSemaphore constructor - Failed to create SDL Semaphore.");
   }

   ~PlatformSemaphore()
   {
       SDL_DestroySemaphore(semaphore);
   }
};

Semaphore::Semaphore(S32 initialCount)
{
  mData = new PlatformSemaphore(initialCount);
}

Semaphore::~Semaphore()
{
  AssertFatal(mData, "Semaphore destructor - Invalid semaphore.");
  delete mData;
}

bool Semaphore::acquire(bool block)
{
   AssertFatal(mData, "Semaphore::acquire - Invalid semaphore.");
   if (block)
   {
      if (SDL_SemWait(mData->semaphore) < 0)
         AssertFatal(false, "Semaphore::acquie - Wait failed.");
      return (true);
   }
   else
   {
      int res = SDL_SemTryWait(mData->semaphore);
      return (res == 0);
   }
}

void Semaphore::release()
{
   AssertFatal(mData, "Semaphore::releaseSemaphore - Invalid semaphore.");
   SDL_SemPost(mData->semaphore);
}
