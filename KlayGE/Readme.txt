ճ����Ϸ����(KlayGE) 2.8.0

Homepage: http://klayge.sourceforge.net
E-Mail:	  enginedev@163.com
OICQ:     33611210

��ӭʹ��ճ����Ϸ���� 2.8.0
�����������KlayGEͷ�ļ���KlayGEԴ���룬KlayGE��VS.NET 2003�����ļ�

KlayGE��һ������Դ����ģ���ƽ̨�ģ����ڲ���ṹ����Ϸ���档

__________________________________________________________________


Ŀ¼

- ʹ��Э��
- ��װ
- �²��ƻ�
- ��չ����
- Ŀ¼�ṹ
- �����ƻ�
- ������֧��


__________________________________________________________________

ʹ��Э��

KlayGE��Ȩ�鹨�������У���2.7.0��ʼ��ѭGPLЭ�顣�������ʹ������
�����뱣֤ʹ��KlayGE����ĿҲ��ѭGPLЭ�顣������ٿ��������з���Bug��
���鷳�������ҡ�

__________________________________________________________________



��װ

1. ��KlayGE.zip��ѹ����һ��Ŀ¼������˵ F:\KlayGE

2. ����·����������VS.NET 2003����������������ο����ǵİ����ļ���
	2.1 �򿪡����ߡ�->��ѡ�->����Ŀ�����ڡ���ʾ�������ݵ�Ŀ¼����������ѡ�񡰰����ļ�����
		���������F:\KlayGE\Core\Include��F:\KlayGE\AppLayer\Include��F:\KlayGE\Plugins\Include
	2.2 �ڡ���ʾ�������ݵ�Ŀ¼����������ѡ��"���ļ�"��
		���������F:\KlayGE\Lib
	2.3 �㡰ȷ������ť������ر�VS.NET 2003 IDE�Ա�������

__________________________________________________________________



����

����KlayGE�ں���ҪһЩ�������⣺Python 2.4.1��Boost 1.32.0��FreeType 2.1.10���ֱ���http://www.python.org��http://boost.sourceforge.net��http://freetype.sourceforge.net����
����DX��ز����ҪDirectX 9.0 SDK June 2005����http://msdn.microsoft.com/directx����
����OggWav�����ҪVorbis SDK����http://www.vorbis.com����
����OpenAL�����ҪOpenAL SDK����http://www.openal.org����


__________________________________________________________________



�²��ƻ�

�ڲ�֧�ֹ�������
��ǿ��Ч�ű���
��ǿOpenGL���


__________________________________________________________________



��չ����

2.8.1 (2005.8.14)
������RenderDeviceCaps
������OpenGL�����Լ�⹤��
������Sampler
��д��RenderEffect
ֻ֧��OpenGL 1.5������
����RenderEngine
DInputDevice��Ϊ��̳нṹ
������RenderToVertexStream

2.7.1 (2005.7.15)
������������ʾЧ��
LoadKMesh����ʹ���Զ�����
������RenderableHelper����
RenderEngine::ViewMatrix��ProjectionMatrix��Ϊconst

2.7.0 (2005.7.2)
�ĳ�GPLЭ��
������KMesh
ȥ����RenderEngine::TextureCoordSet
AddressingMode, Filtering��Anisotropy��RenderEngine�Ƶ�Texture��
Texture������MaxMipLevel��MipMapLodBias
App3D������Quit
֧��OpenGL 1.5

2.6.0 (2005.6.2)
������SceneManager��CanBeCulled��bug
������half����
D3D9Texture�����˶�surface�ļ��
������RenderableSkyBox
֧��HDR

2.5.0 (2005.5.2)
������3DSMax�������
����ͬʱʹ�ö�����붯����
MathLib��Ϊʹ�÷���ֵ���ؽ��
�Ľ���CameraController
������RenderableHelper
��׶�ü���Ϊʹ��LUTʵ��

2.4.0 (2005.3.28)
֧������ѹ��
������D3D9Resource
֧��DDS��ʽ������ͱ���
�˲�����Ϊ����ʵ��
֧���������

2.3.0 (2005.3.2)
ʹ��FreeTypeʵ��Font
�����˼����ڴ�й©
�����˶Ը��������֧��
������CopyToMemory
��������׶�ü����
D3D9���������OnLostDevice��OnResetDevice

2.2.0 (2004.11.20)
������DSound�������������
ȥ����ManagerBase��FileSystem��Crc32��alloc��Timer��Random
ʹ��boost 1.32.0
������Trace

2.1.3 (2004.10.19)
��������boost::tupleΪ������ScriptEngine::Call
������һЩ����STL������
������Pkt��CRC����
ȥ����MemoryLib

2.1.2 (2004.9.5)
������BoneMesh
������RenderEffectParameter
֧��DX 9.0c SDK
������DiskFile::Seek��Bug
��const T&��Ϊ�������T const &
InputEngine����Bridgeʵ��

2.1.1 (2004.5.25)
����ʹ��boost
��ѧ�ⶼ���÷���ʵ��
������Parallax��Demo
������ResLocator

2.1.0 (2004.4.20)
��ǿ��OpenGL
ȥ���˻�����
ֱ��֧�ֵ�����Shader
���˴���ļ�Ŀ¼��ı�ʾ��

2.0.5 (2004.4.13)
��ǿ��OpenGL
������WindowMovedOrResized��bug
MemFile����stringstreamʵ��
����˴��밲ȫ��

2.0.4 (2004.4.7)
������OALMusicBuffer�޷�loop��bug
DSMusicBuffer����timeSetEventʵ��
�Ľ���VertexBuffer
Audio����������NullObject
������Demo

2.0.3 (2004.3.12)
����VertexBuffer
ȥ�������������

2.0.2 (2003.12.28)
�Ľ�����Ⱦ����
ʹ��Python���ű�����
DiskFile��Ϊ�ñ�׼C++��ʵ��

2.0.1 (2003.10.17)
ȥ����DX8��֧�� 
������һЩ����

2.0.0 (2003.10.1)
��ʽ�淢��

2.0.0 Beta (2003.9.5)
���η���


__________________________________________________________________


Ŀ¼�ṹ��


�����ǹ���������KlayGEĿ¼����Ҫ������

\Core
	�ں��ļ�

	\Include
		�ں�ͷ�ļ�

	\Src
		�ں�Դ�ļ�

		\Audio
			��Ƶ�����ں˵�Դ�ļ�
		\Input
			���������ں˵�Դ�ļ�
		\Kernel
			����Դ�ļ�
		\Net
			���������Դ�ļ�
		\Render
			��Ⱦϵͳ�ں˵�Դ�ļ�
		\Script
			�ű������Դ�ļ�
		\Show
			���������ں˵�Դ�ļ�

\Demos
	��ʾ

\Doc
	�ĵ�

\AppLayer
	Ӧ�ó����

	\Include
		Ӧ�ó�����ͷ�ļ�

	\Src
		Ӧ�ó�����Դ�ļ�

\Plugins
	����ļ�

	\Include
		���ͷ�ļ�

	\Src
		���Դ����

		\Audio
			��Ƶ��������Դ�ļ�
		\AudioDataSource
			��Ƶ����Դ�����Դ�ļ�
		\Input
			������������Դ�ļ�
		\PackSystem
			�ļ����ϵͳ��Դ�ļ�
		\Render
			��Ⱦϵͳ�����Դ�ļ�
		\Scene
			���������������Դ�ļ�
		\Show
			������������Դ�ļ�
		\Terrain
			���β����Դ�ļ�

\RenderFX
	��Ⱦ��Ч�ű�

\Tools
	�����ļ�

	\Bin
		�������ɵĹ����ļ�
	\DistanceMapCreator
		��height map����distance map�Ĺ���
	\MeshML2KMesh
		��.MeshML����Ϊ.KMesh�Ĺ���
	\NormalizerCubeMap
		���ɹ�һ��Cube map�Ĺ���
	\NormalMapGen
		��height map����normal map�Ĺ���
	\Pkt
		�ļ��������

\Lib
		��ű����ľ�̬���ӿ�



__________________________________________________________________


������֧��

- �Ƽ�ʹ��Visual Studio.NET 2003����
- KlayGE����Visual Studio.NET 2003�����ͱ���ģ����ṩ��Visual Studio.NET 2003��sln�ļ���
- KlayGE����֤����Visual Studio.NET��ǰ�İ汾�б���ͨ����

