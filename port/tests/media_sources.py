import importlib.util
from pathlib import Path
import tempfile
import unittest
spec=importlib.util.spec_from_file_location('sources',Path(__file__).resolve().parents[1]/'media_sources.py')
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)

class SourcesTests(unittest.TestCase):
    def test_renamed_discs_spaces_and_nested_folders(self):
        with tempfile.TemporaryDirectory() as d:
            root=Path(d);mapping={}
            for n in range(1,7):
                child=root/('folder '+str(n));child.mkdir();p=child/'renamed.ISO';p.touch();mapping[p]=n
            found=m.discover(root,lambda p:mapping[p])
            self.assertEqual(set(found),set(range(1,7)))
    def test_missing_or_duplicate_disc_fails(self):
        with tempfile.TemporaryDirectory() as d:
            root=Path(d)
            with self.assertRaisesRegex(ValueError,'Missing'):m.discover(root,lambda p:1)
            for n in range(2):(root/(str(n)+'.iso')).touch()
            with self.assertRaisesRegex(ValueError,'Ambiguous'):m.discover(root,lambda p:1)
    def test_unrelated_iso_ignored(self):
        with tempfile.TemporaryDirectory() as d:
            root=Path(d);(root/'other.iso').touch()
            def invalid(p):raise ValueError('not game')
            with self.assertRaisesRegex(ValueError,'Missing'):m.discover(root,invalid)
    def test_atomic_replace(self):
        with tempfile.TemporaryDirectory() as d:
            p=Path(d)/'nested/config.json';m.atomic_write(p,'old');m.atomic_write(p,'new')
            self.assertEqual(p.read_text(),'new');self.assertEqual(list(p.parent.iterdir()),[p])
    def test_xwd_24bit_pitch(self):
        import struct, zlib
        p=Path(__file__).resolve().parents[1]/'original_runtime_probe.py'
        spec=importlib.util.spec_from_file_location('probe',p);probe=importlib.util.module_from_spec(spec);spec.loader.exec_module(probe)
        with tempfile.TemporaryDirectory() as d:
            d=Path(d);header=[100,7,2,24,2,1,0,0,32,0,8,24,8,4,0xff0000,0xff00,0xff,8,256,0,2,1,0,0,0]
            source=d/'test.xwd';dest=d/'test.png'
            source.write_bytes(struct.pack('>25I',*header)+bytes([0,0,255,255,0,0,0,0]))
            probe.xwd_png(source,dest);png=dest.read_bytes();at=8;raw=b''
            while at<len(png):
                n=struct.unpack_from('>I',png,at)[0]
                if png[at+4:at+8]==b'IDAT':raw+=png[at+8:at+8+n]
                at+=12+n
            self.assertEqual(zlib.decompress(raw),bytes([0,255,0,0,0,0,255]))

if __name__=='__main__':unittest.main()
