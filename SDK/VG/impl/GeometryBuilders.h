namespace impl
{
	template<class VertexType, VGuint PRIM_COUNT>
	class GeometryBuilder
	{
		protected:
			GeometryBuilder():mBase(InvalidIndex), mCursor(InvalidIndex)
			{}

			VGuint addVertex(const VertexType& v)
			{
				assert(mVertices.size()<(size_t)VG_MAXINT);
				VGuint idx = (VGuint)mVertices.size();
				mVertices.push_back(v);
				calcBBox(v.p);
				return idx;
			}

			void setVertex(VGuint i, const VertexType& v)
			{
				assert(i<mVertices.size());
				mVertices[i] = v;
				calcBBox(v.p);
			}

			void setVertex(VGuint i, const glm::vec2& n)
			{
				mVertices[i].n = n;
			}

			VertexType*	reserveRange(VGuint count, VGuint& begin, VGuint& end)
			{
				assert((mVertices.size()+count)<(size_t)VG_MAXINT);
				
				begin = (VGuint)mVertices.size();
				end   = begin+count;
			
				mVertices.resize(end);
				--end;

				return mVertices.begin()+begin;
			}

			void addPrim(VGuint primType, VGuint i0, VGuint i1, VGuint i2)
			{
				assert(primType<PRIM_COUNT);

				if (i0==i1 || i1==i2 || i2==i0)
					return;

				IndexVector& indices = mIndices[primType];

				indices.push_back(i0);
				indices.push_back(i1);
				indices.push_back(i2);
			}
		
			glm::vec2	getPos(VGuint i)  {return mVertices[i].p;}
			glm::vec2	getNorm(VGuint i) {return mVertices[i].n;}
			bool		isFirstPrim()	  {return mCursor==mBase;}

			void	setTC(VGuint i, const glm::vec3& tc) {mVertices[i].tc = tc;}

		protected:
			void calcBBox(const glm::vec2& p)
			{
				if (mVertices.size()==1)
				{
					mMin = p;
					mMax = p;
				}
				else
				{
					mMin = glm::min(mMin, p);
					mMax = glm::max(mMax, p);
				}
			}

		protected:
			static const VGuint	InvalidIndex = ~((VGuint)0);

		protected:
			Array<VertexType>		mVertices;
			IndexVector				mIndices[PRIM_COUNT];
			VGuint					mBase, mCursor;
			glm::vec2				mMin, mMax;
	};

	class FillGeometryBuilder: public GeometryBuilder<FillVertex, FILL_PRIM_TYPE_COUNT>
	{
		public:
			void begin(VGfloat x, VGfloat y)
			{
				assert(mBase   == InvalidIndex);
				assert(mCursor == InvalidIndex);
				mCursor = mBase = addVertex(FillVertex(x, y));
			}

			void end()
			{
				mCursor = mBase=InvalidIndex;
			}
			
			void lineTo(VGfloat x1, VGfloat y1)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);

				VGuint i1 = addVertex(FillVertex(x1, y1));
				
				addPrim(FILL_PRIM_TYPE_TRI, mBase, mCursor, i1);

				mCursor = i1;
			}

			void addQuad(const Array<glm::vec2>& v)
			{
				assert(v.size()==3);
				
				//TODO: check for degenerate case...

				VGuint i0, end;

				reserveRange(3, i0, end);

				setVertex(i0+0, FillVertex(v[0], 0.0f, 0.0f));
				setVertex(i0+1, FillVertex(v[1], 0.5f, 0.0f));
				setVertex(i0+2, FillVertex(v[2], 1.0f, 1.0f));

				addPrim(FILL_PRIM_TYPE_QUAD, i0, i0+1, i0+2);
			}

			void addCubic(const Array<glm::vec2>& v)
			{
				assert(v.size()==4);

				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;

				cubicTriVertices(v, pos, tc);

				assert(pos.size()==tc.size());
				assert(tc.size()%4==0);
				
				VGuint	begin, end;

				reserveRange((VGuint)pos.size(), begin, end);

				for (size_t i=0; i<pos.size(); ++i)
				{
					setVertex(begin+i, FillVertex(pos[i], tc[i]));
				}

				for (VGuint i0=begin; i0<end; i0+=4)
				{
					addPrim(FILL_PRIM_TYPE_CUBIC, i0,    i0+1, i0+2);
					addPrim(FILL_PRIM_TYPE_CUBIC, i0+2,  i0+3, i0+0);
					addPrim(FILL_PRIM_TYPE_TRI,  begin, i0,   i0+3);
				}
			}

			void addArc(VGPathSegment type, const glm::vec2& radius, VGfloat angle,
					   const glm::vec2& p0, const glm::vec2& p1)
			{
				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;

				arcTriVertices(type, radius, angle, p0, p1, pos, tc);
				
				assert(pos.size()==tc.size());

				VGuint	begin, end;

				reserveRange((VGuint)pos.size(), begin, end);

				for (size_t i=0; i<pos.size(); ++i)
				{
					setVertex(begin+i, FillVertex(pos[i], tc[i]));
				}

				for (VGuint i0=begin; i<end-1; i+=2)
				{
					addPrim(FILL_PRIM_TYPE_ARC,  i0+0,  i0+1, i0+2);
					addPrim(FILL_PRIM_TYPE_TRI,  begin, i0+0, i0+2);
				}
			}
			
			void copyDataTo(FillGeometry& fillGeom)
			{
				VGuint offset = 0, count;

				for (size_t i=0; i<FILL_PRIM_TYPE_COUNT; ++i)
				{
					assert(mIndices[i].size() < VG_MAXINT);
					count = (VGuint)mIndices[i].size();

					fillGeom.offset[i] = offset;
					fillGeom.count[i] = count;
					fillGeom.indices.pushBack(mIndices[i].begin(), mIndices[i].end());

					offset += count;
				}

				fillGeom.vertices.pushBack(mVertices.begin(), mVertices.end());
				fillGeom.mMin = mMin;
				fillGeom.mMax = mMax;
			}

	};

	/* Unlike FillGeometryBuilder all methods should be called between begin/end
	/* order is also important
	*/
	class StrokeGeometryBuilder: public GeometryBuilder<StrokeVertex, STROKE_PRIM_TYPE_COUNT>
	{
		public:
			void begin(VGfloat x, VGfloat y)
			{
				assert(mBase   == InvalidIndex);
				assert(mCursor == InvalidIndex);
				mCursor = mBase = addVertex(StrokeVertex(glm::vec2(x, y)));
			}

			void end(bool closePath)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);

				if (closePath)
				{
					glm::vec2	p0(getPos(mCursor)), p1(getPos(mBase)), n(calcOffset(p0, p1));
					
					VGuint i0 = addVertex(StrokeVertex(p0, n));
					VGuint i1 = addVertex(StrokeVertex(p1, n));
					
					addStroke(i0, i1, mBase);
					addJointBevel(mBase, i1, mBaseN);
					addJointMiter(mBase, i1, mBaseN);
					addJointRound(mBase, i1, mBaseN);
				}
				else
				{
					addCaps(getPos(mBase),    getNorm(mBaseN),   perpendicularCCW(getNorm(mBaseN)));
					addCaps(getPos(mCursor), -getNorm(mCursorN), perpendicularCW (getNorm(mCursorN)));
				}

				mCursor = mBase=InvalidIndex;
			}
			
			void lineTo(VGfloat x1, VGfloat y1)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);

				glm::vec2	p0(getPos(mCursor)), p1(x1, y1), n(calcOffset(p0, p1));
				
				VGuint i0, end;

				reserveRange(3, i0, end);

				setVertex(i0+0, StrokeVertex(p0, n));
				setVertex(i0+1, StrokeVertex(p1, n));
				setVertex(i0+2, StrokeVertex(p1));
				
				addStroke(i0, i0+1, i0+2);
			}

			//Requires 3 vertices, first should be the same as the last in previous primitive
			void quadTo(const Array<glm::vec2>& v)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);
				assert(glm::equal(getPos(mCursor), v[0]));

				glm::vec2	n01(calcOffset(v[0], v[1])), n12(calcOffset(v[1], v[2]));
				glm::vec2	n012(calcOffsetN(n01, n12));
				
				VGuint i0, end;

				reserveRange(4, i0, end);

				setVertex(i0+0, StrokeVertex(v[0],  n01, glm::vec3(0.0f, 0.0f, 0.0f)));
				setVertex(i0+1, StrokeVertex(v[1], n012, glm::vec3(0.5f, 0.0f, 0.0f)));
				setVertex(i0+2, StrokeVertex(v[2],  n12, glm::vec3(1.0f, 1.0f, 0.0f)));
				
				addPrim(STROKE_PRIM_TYPE_QUAD, i0, i0+1, i0+2);

				end = addVertex(StrokeVertex(v[2]));
				addStroke(i0, i0+2, end);
			}

			//Requires 4 vertices, first should be the same as the last in previous primitive
			void cubicTo(const Array<glm::vec2>& v)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);
				assert(glm::equal(getPos(mCursor), v[0]));

				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;
				
				cubicTriVertices(v, pos, tc);

				assert(pos.size()==tc.size());
				assert(tc.size()%4==0);

				VGuint	begin, end;

				reserveRange((VGuint)pos.size(), begin, end);

				for (size_t i=0; i<pos.size(); ++i)
				{
					glm::vec2	n;

					if (i%4==3)
						n = calcOffset(pos[i-1], pos[i]);
					else if (i%4!=0)
						n = calcOffset(pos[i-1], pos[i], pos[i+1]);
					else
						n = calcOffset(pos[i],   pos[i+1]);

					setVertex(begin+i, StrokeVertex(pos[i], n, tc[i]));
				}

				for (VGuint i=begin; i<end; i+=4)
				{
					addPrim(STROKE_PRIM_TYPE_CUBIC, i+0,   i+1, i+2);
					addPrim(STROKE_PRIM_TYPE_CUBIC, i+2,   i+3, i+0);
					addPrim(STROKE_PRIM_TYPE_TRI,   begin, i+0, i+3);
				}

				VGuint i0 = addVertex(StrokeVertex(v[3]));

				addStroke(begin, end, i0);
			}

			void arcTo(VGPathSegment type, const glm::vec2& radius, VGfloat angle, const glm::vec2& p0, const glm::vec2& p1)
			{
				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;

				arcTriVertices(type, radius, angle, p0, p1, pos, tc);
				
				assert(pos.size()==tc.size());

				VGuint	begin, end;

				reserveRange((VGuint)pos.size(), begin, end);

				for (size_t i=0; i<pos.size(); ++i)
				{
					glm::vec2	n;

					if (i==pos.size()-1)
						n = calcOffset(pos[i-1], pos[i]);
					else if (i%2==1)
						n = calcOffset(pos[i-1], pos[i], pos[i+1]);
					else
						n = calcOffset(pos[i],   pos[i+1]);

					setVertex(begin+i, StrokeVertex(pos[i], n, tc[i]));
				}

				for (VGuint i=begin+1; i<end; ++i)
				{
					addPrim(STROKE_PRIM_TYPE_ARC,  begin, i,   i+1);
				}

				VGuint i0 = addVertex(p1);
				addStroke(begin, end, i0);
			}
			
			void copyDataTo(StrokeGeometry& strokeGeom)
			{
				VGuint offset = 0, count;

				for (size_t i=0; i<STROKE_PRIM_TYPE_COUNT; ++i)
				{
					strokeGeom.offset[i] = offset;

					assert(mIndices[i].size() < VG_MAXINT);
					
					count = (VGuint)mIndices[i].size();
					strokeGeom.count[i] = count;
					strokeGeom.indices.pushBack(mIndices[i].begin(), mIndices[i].end());
					offset += count;
				}

				strokeGeom.vertices.pushBack(mVertices.begin(),	mVertices.end());
				strokeGeom.mMin = mMin;
				strokeGeom.mMax = mMax;
			}

		private:
			//Order of indices is very important!!!
			void addStroke(VGuint i1, VGuint i2, VGuint i3)
			{
				addPrim(STROKE_PRIM_TYPE_TRI, mCursor, i1, i2);
				addPrim(STROKE_PRIM_TYPE_TRI, mCursor, i2, i3);

				if (isFirstPrim())
				{
					mBaseN = i1;
				}
				else
				{
					addJointBevel(mCursor, mCursorN, i1);
					addJointMiter(mCursor, mCursorN, i1);
					addJointRound(mCursor, mCursorN, i1);
				}

				mCursorN = i2;
				mCursor  = i3;
			}

			void addJointBevel(VGuint i0, VGuint i1, VGuint i2)
			{
				addPrim(STROKE_PRIM_TYPE_JOINT_BEVEL, i0, i1, i2);
			}

			void addJointMiter(VGuint i0, VGuint i1, VGuint i2)
			{
				glm::vec2	n1(getNorm(i1)), n2(getNorm(i2)), n(calcOffsetN(n1, n2));

				VGuint		idx = addVertex(StrokeVertex(getPos(i0), n));

				addPrim(STROKE_PRIM_TYPE_JOINT_MITER, i1, idx, i2);
			}

			void addJointRound(VGuint i0, VGuint i1, VGuint i2)
			{
				glm::vec2	n1  = getNorm(i1),
							n2  = getNorm(i2),
							nm  = glm::normalize(0.5f*(n1+n2)),
							n1m = calcOffsetN(n1, nm),
							nm2 = calcOffsetN(nm, n2);

				VGuint	begin, end;

				reserveRange(5, begin, end);

				glm::vec2	p0(getPos(i0));

				setVertex(begin+0, StrokeVertex(p0, n1,  glm::vec3(n1,  0)));
				setVertex(begin+1, StrokeVertex(p0, n1m, glm::vec3(n1m, 0)));
				setVertex(begin+2, StrokeVertex(p0, nm,  glm::vec3(nm,  0)));
				setVertex(begin+3, StrokeVertex(p0, nm2, glm::vec3(nm2, 0)));
				setVertex(begin+4, StrokeVertex(p0, n2,  glm::vec3(n2,  0)));

				addPrim(STROKE_PRIM_TYPE_JOINT_ROUND,  begin, begin+1, begin+2);
				addPrim(STROKE_PRIM_TYPE_JOINT_ROUND,  begin, begin+2, begin+3);
				addPrim(STROKE_PRIM_TYPE_JOINT_ROUND,  begin, begin+3, begin+4);
				//addPrim(STROKE_PRIM_TYPE_JOINT_ROUND,  begin,   begin+1, begin+2);
				//addPrim(STROKE_PRIM_TYPE_JOINT_ROUND,  begin+2, begin+3, begin+4);
				//addPrim(STROKE_PRIM_TYPE_JOINT_ROUND,  begin,   begin+2, begin+4);  //Triangle but drawn with arc shader 
			}

			void addCaps(const glm::vec2& p, const glm::vec2& n, const glm::vec2& t)
			{
				VGuint i0 = addVertex(StrokeVertex(p,  -n, glm::vec3( -n, 0)));
				VGuint i1 = addVertex(StrokeVertex(p, t-n, glm::vec3(t-n, 0)));
				VGuint i2 = addVertex(StrokeVertex(p, t+n, glm::vec3(t+n, 0)));
				VGuint i3 = addVertex(StrokeVertex(p,   n, glm::vec3(  n, 0)));

				addPrim(STROKE_PRIM_TYPE_CAP,  i0, i1, i2);
				addPrim(STROKE_PRIM_TYPE_CAP,  i0, i2, i3);
			}

		private:
			VGuint	mBaseN, mCursorN;
	};
}
