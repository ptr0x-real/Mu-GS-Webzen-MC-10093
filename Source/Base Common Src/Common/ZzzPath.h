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
		// ��������� ���°��� ���� -> �켱 �������͵鸸

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
		// ��������� ���°��� ���� -> �켱 �������͵鸸

        //if (LastDir == dirstart)
        int MinDist = 10000000, ldir;
        int WhichDir = 0;
		int i;

		// . > ��ã�� ù �õ��� ���
		if(First)
		{
			for (i=0; i<8; i++)
			{
				// . > 8������ ���Ѵ�.
				ldir=i%8;

				// . > ������ �̿��� ��ǥ ���� +1 ���� ��Ų��.
				//		���ο� X, Y ��ǥ�� ���� �ȴ�.
				int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];

				// . > ���ο� X, Y�� ��ǥ ���������� ����� �����Ѵ�.
				int dist = GetDist(newx, newy, dx, dy);

				// . > �ּ� ����� ��� ���� �����ִµ�,
				// . > IsThisSpotOK �Լ��� �̿��ؼ� �̵� ��ǥ�� �ùٸ��� �Ǵ��Ѵ�.
				//		HitMap �� ��� �Ǿ� �ִ� ��ǥ���� Ȯ���ؼ�, ��ϵǾ� �ִٸ� FALSE
				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{
					// . > �ùٸ� �������� �˻��ؼ� �ùٸ� ��쿡�� �ּҺ������ �����Ѵ�.
					if( CanWeMoveForward3(newx, newy) )
					{
						MinDist		= dist; 
						WhichDir	= ldir;
					}
					
				} 
			}
		}
		// . > ��ã�� �ι�° �õ� ����
		else
		{
			// . > �� ã�� �ι�° �õ� ���ʹ� ���� ��ġ ������ 3���� ���� �˻�
			for (i=dirstart+8-1; i<=dirstart+8+1; i++)
			{
				ldir=i%8;
				int newx = sx+Dir[ldir*2], newy = sy+Dir[ldir*2+1];
				int dist = GetDist(newx, newy, dx, dy);
				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{
					// . > �ùٸ� �������� �˻��ؼ� �ùٸ� ��쿡�� �ּҺ������ �����Ѵ�.
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
						// . > �ùٸ� �������� �˻��ؼ� �ùٸ� ��쿡�� �ּҺ������ �����Ѵ�.
						if( CanWeMoveForward3(newx, newy) )
						{
							MinDist = dist; 
							WhichDir = ldir;
						}
					} 
				}
			}
		}
		
		// . > �̵� ������ ã�� ����
		if (MinDist == 10000000)
			return -1;

		// . > �ּ��� ����� ��� ���� ��ǥ�� ����� �� 
		//		�ʻ翡�� �̵� ������ ��ǥ���� �˾Ƴ���. - ������ �������� �˻�
		if (CanWeMoveForward3(sx+Dir[WhichDir*2], sy+Dir[WhichDir*2+1]))
		{
			// . > ������ �迭�� ���� ��ǥ�� ���� ��� �ش� �迭 ��ġ�� �ű��.
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
		
		// . > �̵� �Ҽ� ���� ��ǥ���, HipMap �� ����صд�.
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