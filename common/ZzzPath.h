#ifndef __ZZZPATH_H__
#define __ZZZPATH_H__

#define MAX_PATH_FIND 15

class PATH
{
private:
    int Width, Height;
    int NumPath;
    unsigned char PathX[500];
    unsigned char PathY[500];
    int Dir[16];
    int LastDir;
    int NumFails;
    unsigned char *Map, *HitMap;
    int stx, sty, edx, edy;

    int GetDist(int x1, int y1, int x2, int y2)
    {
        int distx = x2-x1, disty = y2-y1;
        return distx*distx + disty*disty;
    }
    int VerifyThatOnPath(int x, int y)// in case we cross our path
    {
        for (int i=0; i<NumPath; i++)
        {
            if (x==PathX[i] && y == PathY[i]) return i;
        }
        return -1;
    }
    int CanWeMoveForward(int x, int y)// look here for impassable WALLS
    {
        int which = x+y*Width;
        if (x>=Width || x<0 || y>=Height ||y<0)//off of the map
            return 0;
        if (Map[which]>1 || HitMap[which])
        {          
            HitMap[which] = 1;// fill this area as checked already
            return 0;
        }
        return 1;
    }
#ifdef MONSTER_HERD_SYSTEM_20031120
    int CanWeMoveForward2(int x, int y)// look here for impassable WALLS
    {
        int which = x+y*Width;
        if (x>=Width || x<0 || y>=Height ||y<0)//off of the map
            return 0;
        if (Map[which]>=1 || HitMap[which])
        {          
            HitMap[which] = 1;// fill this area as checked already
            return 0;
        }
        return 1;
    }
#endif
#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	int CanWeMoveForward3(int x, int y)
    {
        int which = x+y*Width;
        if (x>=Width || x<0 || y>=Height ||y<0)
            return 0;
        if (Map[which]>=1 || HitMap[which])
        {          
            HitMap[which] = 1;
            return 0;
        }
        return 1;
    }
#endif
    int IsThisSpotOK(int x, int y)// we're looking strictly at the HitMap
    {
        int pos = x+y*Width;
        if (x<0 || x>=Width || y<0 || y>=Width)
			return 0;
        if (HitMap[pos])
        {
            return 0;
        }        
        return 1;
    }
    int FindNextDir(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck)// this must be smarter
    {
        //if (LastDir == dirstart)
        int MinDist = 10000000, ldir;
        int WhichDir = 0;
		int i;
		if(First)
		{
			for (i=0; i<8; i++)
			{
				ldir=i%8;// only 8 directions in case we wrap
				int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];
				int dist = GetDist(newx, newy, dx, dy);
				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{MinDist = dist; WhichDir = ldir;} 
			}
		}
		else
		{
			for (i=dirstart+8-1; i<=dirstart+8+1; i++)
			{
				ldir=i%8;// only 8 directions in case we wrap
				int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];
				int dist = GetDist(newx, newy, dx, dy);
				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{MinDist = dist; WhichDir = ldir;} 
			}
			if (MinDist == 10000000)//in case we didn't find a spot the first time
			{
				for (i=dirstart+2; i<=dirstart+6; i++)// 5 directions left
				{
					ldir=i%8;
					int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];
					int dist = GetDist(newx, newy, dx, dy);
					if (MinDist > dist && IsThisSpotOK(newx, newy))
					{MinDist = dist; WhichDir = ldir;} 
				}
			}
		}
        LastDir = dirstart;
		int LastDist = GetDist(sx, sy, sx+Dir[LastDir*2], sy+Dir[LastDir*2+1]);
		if (LastDist>MinDist-6 && LastDist<MinDist+6 && MinDist>=25)//???
			WhichDir = LastDir;
		if (MinDist == 10000000)// we could not find a good spot
			return -1;

		// now we know the shortset distance but...
		if (CanWeMoveForward(sx+Dir[WhichDir*2], sy+Dir[WhichDir*2+1]))
		{
			int p = VerifyThatOnPath(sx, sy);
			if (p!=-1)// we are back on a previous point
			{
				HitMap[sx+sy*Width] = 1;
				NumPath = p;
			}
			return WhichDir;
		}
		if(ErrorCheck)
		{
#ifdef MODIFY_PATHFIND_BUGFIX_20090609
			WhichDir += 4; WhichDir %= 8;
#endif	// MODIFY_PATHFIND_BUGFIX_20090609

			int negdir = (WhichDir+8-1); negdir%=8;
			if (CanWeMoveForward(sx+Dir[negdir*2], sy+Dir[negdir*2+1]))
			{
				int p = VerifyThatOnPath(sx, sy);
				if (p!=-1)// we are back on a previous point
				{
					HitMap[sx+sy*Width] = 1;
					NumPath = p;
				}
				return negdir;
			}
			int posdir = (WhichDir+8+1); posdir%=8;
			if (CanWeMoveForward(sx+Dir[posdir*2], sy+Dir[posdir*2+1]))
			{
				int p = VerifyThatOnPath(sx, sy);
				if (p!=-1)// we are back on a previous point
				{
					HitMap[sx+sy*Width] = 1;
					NumPath = p;
				}
				return posdir;
			}
		}
		// if all attempts to move forward were unsuccessful
		// tag the current position and back up one
		HitMap[sx+sy*Width] = 1;
		return -1;// error
	}
#ifdef MONSTER_HERD_SYSTEM_20031120
    int FindNextDir2(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck)// this must be smarter
    {
		// 안전지대로 가는것을 방지 -> 우선 무리몬스터들만

        //if (LastDir == dirstart)
        int MinDist = 10000000, ldir;
        int WhichDir = 0;
		int i;
		if(First)
		{
			for (i=0; i<8; i++)
			{
				ldir=i%8;// only 8 directions in case we wrap
				int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];
				int dist = GetDist(newx, newy, dx, dy);
				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{MinDist = dist; WhichDir = ldir;} 
			}
		}
		else
		{
			for (i=dirstart+8-1; i<=dirstart+8+1; i++)
			{
				ldir=i%8;// only 8 directions in case we wrap
				int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];
				int dist = GetDist(newx, newy, dx, dy);
				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{MinDist = dist; WhichDir = ldir;} 
			}
			if (MinDist == 10000000)//in case we didn't find a spot the first time
			{
				for (i=dirstart+2; i<=dirstart+6; i++)// 5 directions left
				{
					ldir=i%8;
					int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];
					int dist = GetDist(newx, newy, dx, dy);
					if (MinDist > dist && IsThisSpotOK(newx, newy))
					{MinDist = dist; WhichDir = ldir;} 
				}
			}
		}
        LastDir = dirstart;
		int LastDist = GetDist(sx, sy, sx+Dir[LastDir*2], sy+Dir[LastDir*2+1]);
		if (LastDist>MinDist-6 && LastDist<MinDist+6 && MinDist>=25)//???
			WhichDir = LastDir;
		if (MinDist == 10000000)// we could not find a good spot
			return -1;

		// now we know the shortset distance but...
		if (CanWeMoveForward2(sx+Dir[WhichDir*2], sy+Dir[WhichDir*2+1]))
		{
			int p = VerifyThatOnPath(sx, sy);
			if (p!=-1)// we are back on a previous point
			{
				HitMap[sx+sy*Width] = 1;
				NumPath = p;
			}
			return WhichDir;
		}
		if(ErrorCheck)
		{
#ifdef MODIFY_PATHFIND_BUGFIX_20090609
			WhichDir += 4; WhichDir %= 8;
#endif	// MODIFY_PATHFIND_BUGFIX_20090609

			int negdir = (WhichDir+8-1); negdir%=8;
			if (CanWeMoveForward2(sx+Dir[negdir*2], sy+Dir[negdir*2+1]))
			{
				int p = VerifyThatOnPath(sx, sy);
				if (p!=-1)// we are back on a previous point
				{
					HitMap[sx+sy*Width] = 1;
					NumPath = p;
				}
				return negdir;
			}
			int posdir = (WhichDir+8+1); posdir%=8;
			if (CanWeMoveForward2(sx+Dir[posdir*2], sy+Dir[posdir*2+1]))
			{
				int p = VerifyThatOnPath(sx, sy);
				if (p!=-1)// we are back on a previous point
				{
					HitMap[sx+sy*Width] = 1;
					NumPath = p;
				}
				return posdir;
			}
		}
		// if all attempts to move forward were unsuccessful
		// tag the current position and back up one
		HitMap[sx+sy*Width] = 1;
		return -1;// error
	}
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	int FindNextDir3(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck)// this must be smarter
    {
		// 안전지대로 가는것을 방지 -> 우선 무리몬스터들만

        //if (LastDir == dirstart)
        int MinDist = 10000000, ldir;
        int WhichDir = 0;
		int i;

		// . > 길찾기 첫 시도일 경우
		if(First)
		{
			for (i=0; i<8; i++)
			{
				// . > 8방향을 구한다.
				ldir=i%8;

				// . > 방향을 이용해 좌표 값을 +1 증가 시킨다.
				//		새로운 X, Y 좌표가 산출 된다.
				int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];

				// . > 새로운 X, Y와 목표 지점까지의 비용을 산출한다.
				int dist = GetDist(newx, newy, dx, dy);

				// . > 최소 비용일 경우 갱신 시켜주는데,
				// . > IsThisSpotOK 함수를 이용해서 이동 좌표가 올바른지 판단한다.
				//		HitMap 에 기록 되어 있는 좌표인지 확인해서, 기록되어 있다면 FALSE
				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{
					// . > 올바른 영역인지 검사해서 올바를 경우에만 최소비용으로 간주한다.
					if( CanWeMoveForward3(newx, newy) )
					{
						MinDist		= dist; 
						WhichDir	= ldir;
					}
					
				} 
			}
		}
		// . > 길찾기 두번째 시도 부터
		else
		{
			// . > 길 찾기 두번째 시도 부터는 시작 위치 주위의 3방향 먼저 검사
			for (i=dirstart+8-1; i<=dirstart+8+1; i++)
			{
				ldir=i%8;
				int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];
				int dist = GetDist(newx, newy, dx, dy);
				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{
					// . > 올바른 영역인지 검사해서 올바를 경우에만 최소비용으로 간주한다.
					if( CanWeMoveForward3(newx, newy) )
					{
						MinDist = dist; 
						WhichDir = ldir;
					}
				} 
			}
			if (MinDist == 10000000)
			{
				for (i=dirstart+2; i<=dirstart+6; i++)
				{
					ldir=i%8;
					int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];
					int dist = GetDist(newx, newy, dx, dy);
					if (MinDist > dist && IsThisSpotOK(newx, newy))
					{
						// . > 올바른 영역인지 검사해서 올바를 경우에만 최소비용으로 간주한다.
						if( CanWeMoveForward3(newx, newy) )
						{
							MinDist = dist; 
							WhichDir = ldir;
						}
					} 
				}
			}
		}
		
		// . > 이동 지점을 찾지 못함
		if (MinDist == 10000000)
			return -1;

		// . > 최소의 비용이 드는 다음 좌표를 얻었을 때 
		//		맵사에서 이동 가능한 좌표인지 알아낸다. - 못가는 지역인지 검사
		if (CanWeMoveForward3(sx+Dir[WhichDir*2], sy+Dir[WhichDir*2+1]))
		{
			// . > 길차기 배열에 같은 좌표가 있을 경우 해당 배열 위치로 옮긴다.
			int p = VerifyThatOnPath(sx, sy);
			if (p!=-1)// we are back on a previous point
			{
				HitMap[sx+sy*Width] = 1;
				NumPath = p;
			}
			return WhichDir;
		}
		
		if(ErrorCheck)
		{
#ifdef MODIFY_PATHFIND_BUGFIX_20090609
			WhichDir += 4; WhichDir %= 8;
#endif	// MODIFY_PATHFIND_BUGFIX_20090609

			int negdir = (WhichDir+8-1); negdir%=8;
			if (CanWeMoveForward3(sx+Dir[negdir*2], sy+Dir[negdir*2+1]))
			{
				int p = VerifyThatOnPath(sx, sy);
				if (p!=-1)// we are back on a previous point
				{
					HitMap[sx+sy*Width] = 1;
					NumPath = p;
				}
				return negdir;
			}
			int posdir = (WhichDir+8+1); posdir%=8;
			if (CanWeMoveForward3(sx+Dir[posdir*2], sy+Dir[posdir*2+1]))
			{
				int p = VerifyThatOnPath(sx, sy);
				if (p!=-1)// we are back on a previous point
				{
					HitMap[sx+sy*Width] = 1;
					NumPath = p;
				}
				return posdir;
			}
		}
		
		// . > 이동 할수 없는 좌표라면, HipMap 에 기록해둔다.
		HitMap[sx+sy*Width] = 1;
		
		return -1;
	}
#endif

public:
    PATH()
	{
		int dir[] = {-1,-1, 0,-1,  1,-1,  1,0,  1,1,  0,1,  -1,1,  -1,0};// 8dir
		memcpy(Dir, dir, sizeof(int)*16);
	}
	~PATH()
	{
	}
	bool FindPath(int startx, int starty, int endx, int endy, bool ErrorCheck)
	{
		NumPath =0;
		int WhichDir = 0;
		int NumFails = 0;
		
		memset (HitMap, 0, Width*Height);
		
		bool First = true;
		while (startx!=endx || starty!=endy)
		{
			WhichDir = FindNextDir(startx, starty, endx, endy, WhichDir, First, ErrorCheck);
			First = false;
			if (WhichDir >= 0)
			{
				PathX[NumPath] = startx;
				PathY[NumPath] = starty;
				startx += Dir[WhichDir*2  ];
				starty += Dir[WhichDir*2+1];                
				NumPath ++;
				if (NumPath >= MAX_PATH_FIND)
					return false;       
			}
			else
			{
				if(ErrorCheck)
				{
					NumPath --;
					if (NumPath < 0)
						return false;
					startx = PathX[NumPath];
					starty = PathY[NumPath];
					NumFails ++;
					if (NumFails >= 10)
						return false;                               
				}
      			else
					break;
			}
		}
		PathX[NumPath] = startx;
		PathY[NumPath] = starty;
		NumPath ++;
		if (NumPath >= MAX_PATH_FIND)
			return false;       
		return true;
	}
#ifdef MONSTER_HERD_SYSTEM_20031120
	bool FindPath2(int startx, int starty, int endx, int endy, bool ErrorCheck)
	{
		NumPath =0;
		int WhichDir = 0;
		int NumFails = 0;
		
		memset (HitMap, 0, Width*Height);
		
		bool First = true;
		while (startx!=endx || starty!=endy)
		{
			WhichDir = FindNextDir2(startx, starty, endx, endy, WhichDir, First, ErrorCheck);
			First = false;
			if (WhichDir >= 0)
			{
				PathX[NumPath] = startx;
				PathY[NumPath] = starty;
				startx += Dir[WhichDir*2  ];
				starty += Dir[WhichDir*2+1];                
				NumPath ++;
				if (NumPath >= MAX_PATH_FIND-1) {
					NumPath--;
					break;
				}
			}
			else
			{
				if(ErrorCheck)
				{
					NumPath --;
					if (NumPath < 0)
						return false;
					startx = PathX[NumPath];
					starty = PathY[NumPath];
					NumFails ++;
					if (NumFails >= 10)
						return false;                               
				}
      			else
					break;
			}
		}
		PathX[NumPath] = startx;
		PathY[NumPath] = starty;
		NumPath ++;
		if (NumPath >= MAX_PATH_FIND)
			return false;       
		return true;
	}	
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	bool FindPath3(int startx, int starty, int endx, int endy, bool ErrorCheck)
	{
		NumPath =0;
		int WhichDir = 0;
		int NumFails = 0;
		
		memset (HitMap, 0, Width*Height);
		
		bool First = true;
		while (startx!=endx || starty!=endy)
		{
			WhichDir = FindNextDir3(startx, starty, endx, endy, WhichDir, First, ErrorCheck);
			First = false;
			if (WhichDir >= 0)
			{
				PathX[NumPath] = startx;
				PathY[NumPath] = starty;
				startx += Dir[WhichDir*2  ];
				starty += Dir[WhichDir*2+1];                
				NumPath ++;
				if (NumPath >= MAX_PATH_FIND-1) 
				{
					NumPath--;
					break;
				}
			}
			else
			{
				if(ErrorCheck)
				{
					NumPath --;
					if (NumPath < 0)
						return false;
					startx = PathX[NumPath];
					starty = PathY[NumPath];
					NumFails ++;
					if (NumFails >= 10)
						return false;                               
				}
      			else
				{
					break;
				}
			}
		}
		PathX[NumPath] = startx;
		PathY[NumPath] = starty;
		NumPath ++;
		if (NumPath >= MAX_PATH_FIND)
			return false;       
		return true;
	}	
#endif
	
	void SetMapDimensions(int w, int h, unsigned char *map)
	{
		Width = w, Height = h, Map = map;
		HitMap = new unsigned char[Width*Height];
	}
	int GetPath(void)
	{
		return NumPath;
	}
	unsigned char *GetPathX(void)
	{
		return PathX;
	}
	unsigned char *GetPathY(void)
	{
		return PathY;
	}
};

#endif